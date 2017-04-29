#include "parser.h"
#include "parser_state.h"

namespace fp::parse::detail {

parser_state::parser_state(parser& p) : m_parser(p) {}

void parser_state::initialize(const lex::token_view& tokens) {
    it = tokens.begin();
    end = tokens.end();
}

precedence_t parser_state::precedence(lex::token_iterator it) const {
    return m_parser.precedence(it);
}

precedence_t parser_state::precedence(lex::token t) const {
    return m_parser.precedence(t);
}

ast::node parser_state::parse(precedence_t p) { return m_parser.parse(p); }

void parser_state::error(std::string what) const {
    error(it, it + 1, std::move(what));
}

void parser_state::error(
    lex::token_iterator from,
    lex::token_iterator to,
    std::string what
) const {
    throw parse::error(std::move(what), { from, to });
}

} // fp::parse::detail
