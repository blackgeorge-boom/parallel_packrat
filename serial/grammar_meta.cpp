//
// Created by blackgeorge on 4/12/19.
//

#include "peg.h"

void PEG::get_meta(PEG& meta)
{
    // Create a PEG.

    // Create terminals.
    Terminal singleQuote("\'");
    Terminal doubleQuote("\"");
    Terminal backSlash("\\");
    Terminal lessThan("<");
    Terminal minus("-");
    Terminal forwardSlash("/");
    Terminal andTerm("&");
    Terminal notTerm("!");
    Terminal questionMarkTerm("?");
    Terminal starTerm("*");
    Terminal plusTerm("+");
    Terminal openParenTerm("(");
    Terminal closeParenTerm(")");
    Terminal period(".");
    Terminal hash("#");
    Terminal spaceTerm(" ");
    Terminal tab("\t");
    Terminal cr("\r");
    Terminal lf("\n");
    AnyChar anyChar;

    // Create non terminals.
    NonTerminal grammar("Grammar");
    NonTerminal definition("Definition");
    NonTerminal expression("Expression");
    NonTerminal sequence("Sequence");
    NonTerminal prefix("Prefix");
    NonTerminal suffix("Suffix");
    NonTerminal primary("Primary");
    NonTerminal identifier("Identifier");
    NonTerminal identifierStart("IdentifierStart");
    NonTerminal identifierRest("IdentifierRest");
    NonTerminal literal("Literal");
    NonTerminal character("Character");
    NonTerminal leftArrow("LeftArrow");
    NonTerminal slash("Slash");
    NonTerminal and_("And");
    NonTerminal not_("Not");
    NonTerminal questionMark("QuestionMark");
    NonTerminal star("Star");
    NonTerminal plus("Plus");
    NonTerminal openParen("OpenParen");
    NonTerminal closeParen("CloseParen");
    NonTerminal dot("Dot");
    NonTerminal spacing("Spacing");
    NonTerminal comment("Comment");
    NonTerminal space("Space");
    NonTerminal endOfLine("EndOfLine");
    NonTerminal endOfFile("EndOfFile");
    // Create rules.

    // Grammar    <- Spacing Definition+ EndOfFile                     # Type 0
    CompositeExpression grammarExp('\b');
    grammarExp.push_expr(&spacing);
//    grammarExp.push_expr(new CompositeExpression('+', {&definition}));
    grammarExp.push_expr(&endOfFile);
    std::cout << "Check " << meta << "\n";
    meta.push_rule(&grammar, &grammarExp);
    std::cout << "Check2 " << meta << "\n";
/*

    // Definition <- Identifier LEFTARROW Expression                   # Type 1
    CompositeExpression definitionExp = new CompositeExpression(Sequence.get(), identifier, leftArrow, expression);
    Rule definitionRule = new Rule(definition, definitionExp);

    // Expression <- Sequence (SLASH Sequence)*                        # Type 2
    CompositeExpression expressionExp = new CompositeExpression(Sequence.get());
    expressionExp.addSubexpression(sequence);
    CompositeExpression expressionSubExp = new CompositeExpression(Sequence.get(), slash, sequence);
    expressionExp.addSubexpression(new CompositeExpression(Repetition.get(), expressionSubExp));
    Rule expressionRule = new Rule(expression, expressionExp);

    // Sequence   <- Prefix*                                           # Type 3
    CompositeExpression sequenceExp = new CompositeExpression(Repetition.get(), prefix);
    Rule sequenceRule = new Rule(sequence, sequenceExp);

    // Prefix     <- (AND / NOT)? Suffix                               # Type 4
    CompositeExpression prefixExp = new CompositeExpression(Sequence.get());
    CompositeExpression prefixSubExp = new CompositeExpression(Optional.get());
    prefixSubExp.addSubexpression(new CompositeExpression(OrderedChoice.get(), and, not));
    prefixExp.addSubexpression(prefixSubExp);
    prefixExp.addSubexpression(suffix);
    Rule prefixRule = new Rule(prefix, prefixExp);

    // Suffix     <- Primary (QUESTION / STAR / PLUS)?                 # Type 5
    CompositeExpression suffixExp = new CompositeExpression(Sequence.get());
    suffixExp.addSubexpression(primary);
    CompositeExpression suffixSubExp = new CompositeExpression(Optional.get());
    suffixSubExp.addSubexpression(new CompositeExpression(OrderedChoice.get(), questionMark, star, plus));
    suffixExp.addSubexpression(suffixSubExp);
    Rule suffixRule = new Rule(suffix, suffixExp);

    // Primary    <- Identifier !LEFTARROW                             # Type 6
    //		            / OPEN Expression CLOSE
    //		            / Literal / DOT
    CompositeExpression primaryExp = new CompositeExpression(OrderedChoice.get());
    CompositeExpression primarySubExp = new CompositeExpression(Sequence.get());
    primarySubExp.addSubexpression(identifier);
    primarySubExp.addSubexpression(new CompositeExpression(NotFollowedBy.get(), leftArrow));
    primaryExp.addSubexpression(primarySubExp);
    primaryExp.addSubexpression(new CompositeExpression(Sequence.get(), openParen, expression, closeParen));
    primaryExp.addSubexpression(literal);
    primaryExp.addSubexpression(dot);
    Rule primaryRule = new Rule(primary, primaryExp);

    // Identifier <- '[a-zA-Z_][a-zA-Z_0-9]*' Spacing                  # Type 7
    CompositeExpression identifierExp = new CompositeExpression(Sequence.get());
    identifierExp.addSubexpression(identifierStart);
    identifierExp.addSubexpression(new CompositeExpression(Repetition.get(), identifierRest));
    identifierExp.addSubexpression(spacing);
    Rule identifierRule = new Rule(identifier, identifierExp);

    CompositeExpression identifierStartExp = new CompositeExpression(OrderedChoice.get(), identifierCharacters);
    Rule identifierStartRule = new Rule(identifierStart, identifierStartExp);

    CompositeExpression identifierRestExp = new CompositeExpression(OrderedChoice.get(), identifierCharacters);
    identifierRestExp.addSubexpressions(digits);
    Rule identifierRestRule = new Rule(identifierRest, identifierRestExp);

    // Literal    <- "'" (!"'" Char)* "'" Spacing                      # Type 8
    //             / '"' (!'"' Char)* '"' Spacing
    CompositeExpression literalExp = new CompositeExpression(OrderedChoice.get());
    CompositeExpression literalFirstOption = new CompositeExpression(Sequence.get());
    literalFirstOption.addSubexpression(singleQuote);
    CompositeExpression literalRepetition = new CompositeExpression(Repetition.get());
    CompositeExpression literalSequenceinRepetition = new CompositeExpression(Sequence.get());
    literalSequenceinRepetition.addSubexpression(new CompositeExpression(NotFollowedBy.get(), singleQuote));
    literalSequenceinRepetition.addSubexpression(character);
    literalRepetition.addSubexpression(literalSequenceinRepetition);
    literalFirstOption.addSubexpression(literalRepetition);
    literalFirstOption.addSubexpression(singleQuote);
    literalFirstOption.addSubexpression(spacing);
    literalExp.addSubexpression(literalFirstOption);
    CompositeExpression literalSecondOption = new CompositeExpression(Sequence.get());
    literalSecondOption.addSubexpression(doubleQuote);
    CompositeExpression literalRepetition2 = new CompositeExpression(Repetition.get());
    CompositeExpression literalSequenceinRepetition2 = new CompositeExpression(Sequence.get());
    literalSequenceinRepetition2.addSubexpression(new CompositeExpression(NotFollowedBy.get(), doubleQuote));
    literalSequenceinRepetition2.addSubexpression(character);
    literalRepetition2.addSubexpression(literalSequenceinRepetition2);
    literalSecondOption.addSubexpression(literalRepetition2);
    literalSecondOption.addSubexpression(doubleQuote);
    literalSecondOption.addSubexpression(spacing);
    literalExp.addSubexpression(literalSecondOption);
    Rule literalRule = new Rule(literal, literalExp);

    // Char       <- '\\[nrt\'"\+\-\*\.\?\^\$\{\}\(\)\[\]\\]'          # Type 9
    //             / '\\[0-2][0-7][0-7]'
    //             / '\\[0-7][0-7]?'
    //             / !'\\' .
    CompositeExpression characterExp = new CompositeExpression(OrderedChoice.get());
    CompositeExpression characterSubExp = new CompositeExpression(Sequence.get());
    characterSubExp.addSubexpression(backSlash);
    characterSubExp.addSubexpression(new CompositeExpression(OrderedChoice.get(), escapableCharacters));
    characterExp.addSubexpression(characterSubExp);
    CompositeExpression characterSubExp2 = new CompositeExpression(Sequence.get());
    characterSubExp2.addSubexpression(new CompositeExpression(NotFollowedBy.get(), backSlash));
    characterSubExp2.addSubexpression(anyCharacter);
    characterExp.addSubexpression(characterSubExp2);
    Rule characterRule = new Rule(character, characterExp);

    // LEFTARROW  <- '<-' Spacing                                      # Type 10
    CompositeExpression leftArrowExp = new CompositeExpression(Sequence.get(), lessThan, minus, spacing);
    Rule leftArrowRule = new Rule(leftArrow, leftArrowExp);

    // SLASH      <- '/' Spacing                                       # Type 11
    CompositeExpression slashExp = new CompositeExpression(Sequence.get(), forwardSlash, spacing);
    Rule slashRule = new Rule(slash, slashExp);

    // AND        <- '&' Spacing                                       # Type 12
    CompositeExpression andExp = new CompositeExpression(Sequence.get(), andTerm, spacing);
    Rule andRule = new Rule(and, andExp);

    // NOT        <- '!' Spacing                                       # Type 13
    CompositeExpression notExp = new CompositeExpression(Sequence.get(), notTerm, spacing);
    Rule notRule = new Rule(not, notExp);

    // QUESTION   <- '\\?' Spacing                                     # Type 14
    CompositeExpression questionMarkExp = new CompositeExpression(Sequence.get(), questionMarkTerm, spacing);
    Rule questionMarkRule = new Rule(questionMark, questionMarkExp);

    // STAR       <- '\\*' Spacing                                     # Type 15
    CompositeExpression starExp = new CompositeExpression(Sequence.get(), starTerm, spacing);
    Rule starRule = new Rule(star, starExp);

    // PLUS       <- '\\+' Spacing                                     # Type 16
    CompositeExpression plusExp = new CompositeExpression(Sequence.get(), plusTerm, spacing);
    Rule plusRule = new Rule(plus, plusExp);

    // OPEN       <- '\\(' Spacing                                     # Type 17
    CompositeExpression openParenExp = new CompositeExpression(Sequence.get(), openParenTerm, spacing);
    Rule openParenRule = new Rule(openParen, openParenExp);

    // CLOSE      <- '\\)' Spacing                                     # Type 18
    CompositeExpression closeParenExp = new CompositeExpression(Sequence.get(), closeParenTerm, spacing);
    Rule closeParenRule = new Rule(closeParen, closeParenExp);

    // DOT        <- '\\.' Spacing                                     # Type 19
    CompositeExpression dotExp = new CompositeExpression(Sequence.get(), period, spacing);
    Rule dotRule = new Rule(dot, dotExp);

    // Spacing    <- (Space / Comment)*                                # Type 20
    CompositeExpression spacingExp = new CompositeExpression(Repetition.get());
    spacingExp.addSubexpression(new CompositeExpression(OrderedChoice.get(), space, comment));
    Rule spacingRule = new Rule(spacing, spacingExp);

    // Comment    <- '#' (!EndOfLine .)* EndOfLine                     # Type 21
    CompositeExpression commentExp = new CompositeExpression(Sequence.get());
    commentExp.addSubexpression(hash);
    CompositeExpression commentSubExp = new CompositeExpression(Sequence.get());
    commentSubExp.addSubexpression(new CompositeExpression(NotFollowedBy.get(), endOfLine));
    commentSubExp.addSubexpression(anyCharacter);
    commentExp.addSubexpression(new CompositeExpression(Repetition.get(), commentSubExp));
    commentExp.addSubexpression(endOfLine);
    Rule commentRule = new Rule(comment, commentExp);

    // Space      <- ' ' / '\t' / EndOfLine                            # Type 22
    CompositeExpression spaceExp = new CompositeExpression(OrderedChoice.get(), spaceTerm, tab, endOfLine);
    Rule spaceRule = new Rule(space, spaceExp);

    // EndOfLine  <- '\r\n' / '\n' / '\r'                              # Type 23
    CompositeExpression endOfLineExp = new CompositeExpression(OrderedChoice.get());
    endOfLineExp.addSubexpression(new CompositeExpression(Sequence.get(), cr, lf));
    endOfLineExp.addSubexpression(lf);
    endOfLineExp.addSubexpression(cr);
    Rule endOfLineRule = new Rule(endOfLine, endOfLineExp);

    // EndOfFile  <- !.                                                # Type 24
    CompositeExpression endOfFileExp = new CompositeExpression(NotFollowedBy.get(), anyCharacter);
    Rule endOfFileRule = new Rule(endOfFile, endOfFileExp);

    // Add the rules to the PEG.
    peg.addRule(grammarRule);
    peg.addRule(definitionRule);
    peg.addRule(expressionRule);
    peg.addRule(sequenceRule);
    peg.addRule(prefixRule);
    peg.addRule(suffixRule);
    peg.addRule(primaryRule);
    peg.addRule(identifierRule);
    peg.addRule(identifierStartRule);
    peg.addRule(identifierRestRule);
    peg.addRule(literalRule);
    peg.addRule(characterRule);
    peg.addRule(leftArrowRule);
    peg.addRule(slashRule);
    peg.addRule(andRule);
    peg.addRule(notRule);
    peg.addRule(questionMarkRule);
    peg.addRule(starRule);
    peg.addRule(plusRule);
    peg.addRule(openParenRule);
    peg.addRule(closeParenRule);
    peg.addRule(dotRule);
    peg.addRule(spacingRule);
    peg.addRule(commentRule);
    peg.addRule(spaceRule);
    peg.addRule(endOfLineRule);
    peg.addRule(endOfFileRule);

*/
    // Set the start symbol.
    meta.set_start(&grammar);
}

