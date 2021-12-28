import AE

class Interpreter:
    def interp(self, ast):
        if isinstance(ast, AE.Num):
            # print(f'interp add : {ast}')
            return ast.getStrNum()
        
        if isinstance(ast, AE.Add):
            # print(f'interp add : {ast}')
            result =  self.atoi(self.interp(ast.getLhs())) + self.atoi(self.interp(ast.getRhs()))
            return str(result)
        
        if isinstance(ast, AE.Sub):
            # print(f'interp add : {ast}')
            result =  self.atoi(self.interp(ast.getLhs())) - self.atoi(self.interp(ast.getRhs()))
            return str(result)

    def atoi(self, num):
        if "-" in num :
            num = num.replace("-", "")
            return -1 * int(num)
        else:
            return int(num)

