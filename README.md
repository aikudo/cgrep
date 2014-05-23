#Program specification

The program is presented in the form of a Unix man(1) page.

NAME
     jgrep -- search files for a pattern

SYNOPSIS
     jgrep [-ilnv] pattern [filename...]

DESCRIPTION
     The jgrep utility searches text files for a pattern and prints
     out all lines that match that pattern.  Patterns are specified as
     regular expressions and make use of java.util.regex.Pattern.

OPTIONS
     The options word must be the first word and begin with a minus
     sign (-) if present.  Option letters in the options word may
     occur in any order.  Operands appear thereafter.

     -i   Ignore upper- and lower-case distinctions in the specified
          pattern.

     -l   Prints only the names of the files with matching lines, one
          filename per line.  A filename is printed only once.

     -n   Precede each line by its line number within the file, with
          the first line being line 1.

     -v   Prints all lines except for those that match the pattern.
          The sense of the match is complemented.

OPERANDS
     The first operand is required and is a regular expression
     according to the syntax recognized by the java.util.regex.Pattern
     class.  All other operands are filenames.  Each file is opened
     for input in turn.  if no filenames are specified, then stdin is
     read instead.

EXIT STATUS
     The following exit status values are returned when the program
     exits:

     0    One or more matches were found.

     1    No matches were found.

     2    Syntax errors or inaccessible files were found.

SEE ALSO
     grep(1), egrep(1), fgrep(1).

