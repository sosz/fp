#pragma once

#include <fp/ast/node.h>
#include <fp/parse/detail/parser_state.h>
#include <fp/parse/detail/parsers/binary_op.h>
#include <fp/parse/detail/parsers/postfix_op.h>
#include <fp/parse/detail/token_table_t.h>

namespace fp::parse::detail {

using infix_parser_t = ast::node (*)(parser_state&, ast::node);

inline ast::node infix_parser_error(parser_state& s, ast::node) {
    s.error();
    ++s.it;
    return ast::make<ast::error>(s.it - 1, s.it);
}

inline ast::node infix_parser_skip_error(parser_state& s, ast::node) {
    ++s.it;
    return ast::make<ast::error>(s.it - 1, s.it);
}

constexpr auto infix_parser_table = ([]() {
    auto t = token_table_t<infix_parser_t>::with_default(infix_parser_error);

    // binary-operators
    t[lex::token::COMMA] = parsers::binary_op;
    t[lex::token::ANNOTATION] = parsers::binary_op;
    t[lex::token::SCOPE] = parsers::binary_op;
    t[lex::token::SEMICOLON] = parsers::binary_op;
    t[lex::token::MEMBER_ACCESS] = parsers::binary_op;
    t[lex::token::RANGE] = parsers::binary_op;
    t[lex::token::CLOSED_RANGE] = parsers::binary_op;
    t[lex::token::TYPE_ARROW] = parsers::binary_op;
    t[lex::token::LAMBDA_ARROW] = parsers::binary_op;
    t[lex::token::PLUS] = parsers::binary_op;
    t[lex::token::MINUS] = parsers::binary_op;
    t[lex::token::MUL] = parsers::binary_op;
    t[lex::token::DIV] = parsers::binary_op;
    t[lex::token::MOD] = parsers::binary_op;
    t[lex::token::POW] = parsers::binary_op;
    t[lex::token::BIT_AND] = parsers::binary_op;
    t[lex::token::BIT_OR] = parsers::binary_op;
    t[lex::token::XOR] = parsers::binary_op;
    t[lex::token::LSHIFT] = parsers::binary_op;
    t[lex::token::RSHIFT] = parsers::binary_op;
    t[lex::token::ASSIGN] = parsers::binary_op;
    t[lex::token::PLUS_ASSIGN] = parsers::binary_op;
    t[lex::token::MINUS_ASSIGN] = parsers::binary_op;
    t[lex::token::MUL_ASSIGN] = parsers::binary_op;
    t[lex::token::DIV_ASSIGN] = parsers::binary_op;
    t[lex::token::MOD_ASSIGN] = parsers::binary_op;
    t[lex::token::POW_ASSIGN] = parsers::binary_op;
    t[lex::token::BIT_AND_ASSIGN] = parsers::binary_op;
    t[lex::token::BIT_OR_ASSIGN] = parsers::binary_op;
    t[lex::token::XOR_ASSIGN] = parsers::binary_op;
    t[lex::token::LSHIFT_ASSIGN] = parsers::binary_op;
    t[lex::token::RSHIFT_ASSIGN] = parsers::binary_op;
    t[lex::token::EQ] = parsers::binary_op;
    t[lex::token::NE] = parsers::binary_op;
    t[lex::token::LT] = parsers::binary_op;
    t[lex::token::GT] = parsers::binary_op;
    t[lex::token::LTE] = parsers::binary_op;
    t[lex::token::GTE] = parsers::binary_op;

    // postfix-operators
    t[lex::token::OPTIONAL] = parsers::postfix_op;
    t[lex::token::INC] = parsers::postfix_op;
    t[lex::token::DEC] = parsers::postfix_op;

    // error
    t[lex::token::ERROR] = infix_parser_skip_error;

    return t;
})();

ast::node parse_infix(parser_state& s, ast::node lhs) {
    ast::node node = infix_parser_table[s.it->value](s, std::move(lhs));
    if (node.is<ast::error>()) {
        // try to recover from the error by skipping over all non-infix tokens,
        // while ignoring all errors along the way to avoid diagnostics clutter
        WITH(s.ignore_diagnostics_in_scope()) {
            while (
                s.it != s.end &&
                infix_parser_table[s.it->value] == infix_parser_error
            ) {
                prefix_parser_table[s.it->value](s);
            }
        }
    }
    return node;
}

} // namespace fp::parse::detail
