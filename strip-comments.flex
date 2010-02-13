/* scanner to strip out as many unecessary characters as possible from C and
 * C++ code
 */

%option noyywrap nounput

CharacterConstant     "' '"
TemplateVariables     "<"[a-zA-Z][a-zA-Z0-9]*">"[ \t]+[a-zA-Z][a-zA-Z0-9]*
NestedTemplates       ("<"[ \t]"<")|(">"[ \t]">"" "?)
PrecompilerDefines    ([\n]?[ \t]*"#".*[\n\r])
SpacedKeywords        ([a-zA-Z0-9][ \t]+[&*]*[a-zA-Z0-9])
NecessaryBlankspace   {CharacterConstant}|{TemplateVariables}|{NestedTemplates}|{PrecompilerDefines}|{SpacedKeywords}
LeadingBlankspace     ^[ \t]+
Blankspace            [ \t\n]
Comment               ("/*"([^*]|[\r\n]|("*"+([^*/]|[\r\n])))*"*"+"/")|("//".*)

%%

{NecessaryBlankspace} { printf("%s", yytext); }
{Comment}             { 42;                   }
{Blankspace}          { 42;                   }
{LeadingBlankspace}   { 42;                   }

.                     { printf("%s", yytext); }

%%


