import sys
import getopt
import Parser
import AE
import Interpreter


def main():
    try:
        options, args = getopt.getopt(sys.argv[1:], "p")
    except getopt.GetoptError as err:
        print(str(err))
        sys.exit(1)

    if len(args) != 1:
        print("wrong input argument")
        sys.exit(1)

    con_code = args[0]
    p = False
    for opt, arg in options:
        if opt == '-p':
            p = True
        else:
            print("wrong input argument")
            sys.exit(1)

    parser = Parser.Parser()
    ast = parser.parse(con_code)

    if ast is None:
        print("Syntax Error!")
        sys.exit(1)

    result = None
    if p:
        print(ast.getASTCodes())
    else:
        interpreter = Interpreter.Interpreter()
        result = interpreter.interp(ast)
        print(result)



if __name__ == "__main__":
    main()