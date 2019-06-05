//
// Created by blackgeorge on 4/12/19.
//

#include "meta_grammar.h"

Meta::Meta() : PEG()
{
    // Create terminals.
    static Terminal singleQuote("\'");
    static Terminal doubleQuote("\"");
    static Terminal backSlash("\\");
    static Terminal lessThan("<");
    static Terminal minus("-");
    static Terminal forwardSlash("/");
    static Terminal andTerm("&");
    static Terminal notTerm("!");
    static Terminal questionMarkTerm("?");
    static Terminal starTerm("*");
    static Terminal plusTerm("+");
    static Terminal openParenTerm("(");
    static Terminal closeParenTerm(")");
    static Terminal period(".");
    static Terminal hash("#");
    static Terminal spaceTerm(" ");
    static Terminal tab("\t");
    static Terminal cr("\r");
    static Terminal lf("\n");
    static AnyChar anyChar;

    // Create non terminals.
    static NonTerminal grammar("Grammar");
    static NonTerminal definition("Definition");
    static NonTerminal expression("Expression");
    static NonTerminal sequence("Sequence");
    static NonTerminal prefix("Prefix");
    static NonTerminal suffix("Suffix");
    static NonTerminal primary("Primary");
    static NonTerminal identifier("Identifier");
    static NonTerminal identifierStart("IdentifierStart");
    static NonTerminal identifierRest("IdentifierRest");
    static NonTerminal literal("Literal");
    static NonTerminal character("Character");
    static NonTerminal leftArrow("LeftArrow");
    static NonTerminal slash("Slash");
    static NonTerminal and_("And");
    static NonTerminal not_("Not");
    static NonTerminal questionMark("QuestionMark");
    static NonTerminal star("Star");
    static NonTerminal plus("Plus");
    static NonTerminal openParen("OpenParen");
    static NonTerminal closeParen("CloseParen");
    static NonTerminal dot("Dot");
    static NonTerminal spacing("Spacing");
    static NonTerminal comment("Comment");
    static NonTerminal space("Space");
    static NonTerminal endOfLine("EndOfLine");
    static NonTerminal endOfFile("EndOfFile");

    // Create rules.

    // Grammar <- Spacing Definition+ EndOfFile                     # Type 0
    static CompositeExpression grammarExp('\b');
    grammarExp.push_expr(&spacing);
    grammarExp.push_expr(new CompositeExpression('+', {&definition}));
    grammarExp.push_expr(&endOfFile);
    this->push_rule(&grammar, &grammarExp);

    // Definition <- Identifier LEFTARROW Expression                # Type 1
    static CompositeExpression definitionExp('\b', {&identifier, &leftArrow, &expression});
    this->push_rule(&definition, &definitionExp);

    // Expression <- Sequence (SLASH Sequence)*                     # Type 2
    static CompositeExpression expressionExp('\b');
    expressionExp.push_expr(&sequence);
    static CompositeExpression expressionSubExp('\b', {&slash, &sequence});
    expressionExp.push_expr(new CompositeExpression('*', {&expressionSubExp}));
    this->push_rule(&expression, &expressionExp);

    // Sequence <- Prefix*                                          # Type 3
    static CompositeExpression sequenceExp('*', {&prefix});
    this->push_rule(&sequence, &sequenceExp);

    // Prefix <- (AND / NOT)? Suffix                                # Type 4
    static CompositeExpression prefixExp('\b');
    static CompositeExpression prefixSubExp('?');
    prefixSubExp.push_expr(new CompositeExpression('/', {&and_, &not_}));
    prefixExp.push_expr(&prefixSubExp);
    prefixExp.push_expr(&suffix);
    this->push_rule(&prefix, &prefixExp);

    // Suffix <- Primary (QUESTION / STAR / PLUS)?                  # Type 5
    static CompositeExpression suffixExp('\b');
    suffixExp.push_expr(&primary);
    static CompositeExpression suffixSubExp('?');
    suffixSubExp.push_expr(new CompositeExpression('/', {&questionMark, &star, &plus}));
    suffixExp.push_expr(&suffixSubExp);
    this->push_rule(&suffix, &suffixExp);

    // Primary <- Identifier !LEFTARROW                             # Type 6
    //		    / OPEN Expression CLOSE
    //		    / Literal / DOT
    static CompositeExpression primaryExp('/');
    static CompositeExpression primarySubExp('\b');
    primarySubExp.push_expr(&identifier);
    primarySubExp.push_expr(new CompositeExpression('!', {&leftArrow}));
    primaryExp.push_expr(&primarySubExp);
    primaryExp.push_expr(new CompositeExpression('\b', {&openParen, &expression, &closeParen}));
    primaryExp.push_expr(&literal);
    primaryExp.push_expr(&dot);
    this->push_rule(&primary, &primaryExp);

    // Identifier <- '[a-zA-Z_][a-zA-Z_0-9]*' Spacing               # Type 7
    static CompositeExpression identifierExp('\b');
    identifierExp.push_expr(&identifierStart);
    identifierExp.push_expr(new CompositeExpression('*', {&identifierRest}));
    identifierExp.push_expr(&spacing);
    this->push_rule(&identifier, &identifierExp);

    static CompositeExpression identifierStartExp('/', "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ_");
    this->push_rule(&identifierStart, &identifierStartExp);

    static CompositeExpression identifierRestExp('/', "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ_0123456789");
    this->push_rule(&identifierRest, &identifierRestExp);

    // Literal    <- "'" (!"'" Char)* "'" Spacing                   # Type 8
    //             / '"' (!'"' Char)* '"' Spacing
    static CompositeExpression literalExp('/');
    static CompositeExpression literalFirstOption('\b');
    literalFirstOption.push_expr(&singleQuote);
    static CompositeExpression literalRepetition('*');
    static CompositeExpression literalSequenceInRepetition('\b');
    literalSequenceInRepetition.push_expr(new CompositeExpression('!', {&singleQuote}));
    literalSequenceInRepetition.push_expr(&character);
    literalRepetition.push_expr(&literalSequenceInRepetition);
    literalFirstOption.push_expr(&literalRepetition);
    literalFirstOption.push_expr(&singleQuote);
    literalFirstOption.push_expr(&spacing);
    literalExp.push_expr(&literalFirstOption);
    static CompositeExpression literalSecondOption('\b');
    literalSecondOption.push_expr(&doubleQuote);
    static CompositeExpression literalRepetition2('*');
    static CompositeExpression literalSequenceInRepetition2('\b');
    literalSequenceInRepetition2.push_expr(new CompositeExpression('!', {&doubleQuote}));
    literalSequenceInRepetition2.push_expr(&character);
    literalRepetition2.push_expr(&literalSequenceInRepetition2);
    literalSecondOption.push_expr(&literalRepetition2);
    literalSecondOption.push_expr(&doubleQuote);
    literalSecondOption.push_expr(&spacing);
    literalExp.push_expr(&literalSecondOption);
    this->push_rule(&literal, &literalExp);

    // Char       <- '\\[nrt\'"\+\-\*\.\?\^\$\{\}\(\)\[\]\\]'       # Type 9
    //             / '\\[0-2][0-7][0-7]'
    //             / '\\[0-7][0-7]?'
    //             / !'\\' .
    static CompositeExpression characterExp('/');
    static CompositeExpression characterSubExp('\b');
    characterSubExp.push_expr(&backSlash);
    characterSubExp.push_expr(new CompositeExpression('/', "nrt\\'\""));
    characterExp.push_expr(&characterSubExp);
    static CompositeExpression characterSubExp2('\b');
    characterSubExp2.push_expr(new CompositeExpression('!', {&backSlash}));
    characterSubExp2.push_expr(&anyChar);
    characterExp.push_expr(&characterSubExp2);
    this->push_rule(&character, &characterExp);

    // LEFTARROW  <- '<-' Spacing                                   # Type 10
    static CompositeExpression leftArrowExp('\b', {&lessThan, &minus, &spacing});
    this->push_rule(&leftArrow, &leftArrowExp);

    // SLASH      <- '/' Spacing                                    # Type 11
    static CompositeExpression slashExp('\b', {&forwardSlash, &spacing});
    this->push_rule(&slash, &slashExp);

    // AND        <- '&' Spacing                                    # Type 12
    static CompositeExpression and_Exp('\b', {&andTerm, &spacing});
    this->push_rule(&and_, &and_Exp);

    // NOT        <- '!' Spacing                                    # Type 13
    static CompositeExpression not_Exp('\b', {&notTerm, &spacing});
    this->push_rule(&not_, &not_Exp);

    // QUESTION   <- '\\?' Spacing                                  # Type 14
    static CompositeExpression questionMarkExp('\b', {&questionMarkTerm, &spacing});
    this->push_rule(&questionMark, &questionMarkExp);

    // STAR       <- '\\*' Spacing                                  # Type 15
    static CompositeExpression starExp('\b', {&starTerm, &spacing});
    this->push_rule(&star, &starExp);

    // PLUS       <- '\\+' Spacing                                  # Type 16
    static CompositeExpression plusExp('\b', {&plusTerm, &spacing});
    this->push_rule(&plus, &plusExp);

    // OPEN       <- '\\(' Spacing                                  # Type 17
    static CompositeExpression openParenExp('\b', {&openParenTerm, &spacing});
    this->push_rule(&openParen, &openParenExp);

    // CLOSE      <- '\\)' Spacing                                  # Type 18
    static CompositeExpression closeParenExp('\b', {&closeParenTerm, &spacing});
    this->push_rule(&closeParen, &closeParenExp);

    // DOT        <- '\\.' Spacing                                  # Type 19
    static CompositeExpression dotExp('\b', {&period, &spacing});
    this->push_rule(&dot, &dotExp);

    // Spacing    <- (Space / Comment)*                             # Type 20
    static CompositeExpression spacingExp('*');
    spacingExp.push_expr(new CompositeExpression('/', {&space, &comment}));
    this->push_rule(&spacing, &spacingExp);

    // Comment    <- '#' (!EndOfLine .)* EndOfLine                  # Type 21
    static CompositeExpression commentExp('\b');
    commentExp.push_expr(&hash);
    static CompositeExpression commentSubExp('\b');
    commentSubExp.push_expr(new CompositeExpression('!', {&endOfLine}));
    commentSubExp.push_expr(&anyChar);
    commentExp.push_expr(new CompositeExpression('*', {&commentSubExp}));
    commentExp.push_expr(&endOfLine);
    this->push_rule(&comment, &commentExp);

    // Space      <- ' ' / '\t' / EndOfLine                         # Type 22
    static CompositeExpression spaceExp('/', {&spaceTerm, &tab, &endOfLine});
    this->push_rule(&space, &spaceExp);

    // EndOfLine  <- '\r\n' / '\n' / '\r'                           # Type 23
    static CompositeExpression endOfLineExp('/');
    endOfLineExp.push_expr(new CompositeExpression('\b', {&cr, &lf}));
    endOfLineExp.push_expr(&lf);
    endOfLineExp.push_expr(&cr);
    this->push_rule(&endOfLine, &endOfLineExp);

    // EndOfFile  <- !.                                             # Type 24
    static CompositeExpression endOfFileExp('!', {&anyChar});
    this->push_rule(&endOfFile, &endOfFileExp);

   // Set the start symbol.
    this->set_start(&grammar);
}

