import Parser


class WAE:
    """super  class"""
    astCode = ""
    def getASTCodes(self):
        if isinstance(self, Num):
            astCode = self.getASTCode()

        if isinstance(self, Add):
            astCode = self.getASTCode()

        if isinstance(self, Sub):
            astCode = self.getASTCode()
        
        if isinstance(self, With):
            astCode = self.getASTCode()

        return astCode

class Num(WAE):
    def __init__(self, number):
        self.n = number

    def getStrNum(self):
        return self.n ;

    def getASTCode(self):
        # print(f'in Num: {self}')
        return "(num " + self.n +")"


class Add(WAE):
    def __init__(self, lhs, rhs):
        self.lhs = lhs
        self.rhs = rhs
    
    def getLhs(self):
        return self.lhs
    
    def getRhs(self):
        return self.rhs

    def getASTCode(self):
        # print(f'in Add: {self}')
        return "(add " + self.lhs.getASTCode() + " " + self.rhs.getASTCode() + ")"


class Sub(WAE):

    def __init__(self, lhs, rhs):
        self.lhs = lhs
        self.rhs = rhs
    
    def getLhs(self):
        return self.lhs
    
    def getRhs(self):
        return self.rhs

    def getASTCode(self):
        # print(f'in Sub: {self}')
        return "(sub " + self.lhs.getASTCode() + " " + self.rhs.getASTCode() + ")"

class With(WAE):
    def __init__(self, name, named_expr, body):
        self.name = name
        self.named_expr = named_expr
        self.body = body

    def getName(self):
        return self.name

    def getNamed_expr(self):
        return self.named_expr
    
    def getBody(self):
        return self.body
    
    def getASTCode(self):
        return "(with " + "'" + self.name + " " + self.named_expr.getASTCode() + " " + self.body.getASTCode() + ")"


class Id(WAE):
    def __init__(self, name):
        self.name = name

    def getId(self):
        return self.name ;

    def getASTCode(self):
        # print(f'in Num: {self}')
        return "(id '" + self.name + ")"
    
