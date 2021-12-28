import Parser


class AE:
    """super  class"""
    astCode = ""
    def getASTCodes(self):
        if isinstance(self, Num):
            astCode = self.getASTCode()

        if isinstance(self, Add):
            astCode = self.getASTCode()

        if isinstance(self, Sub):
            astCode = self.getASTCode()

        return astCode

class Num(AE):
    def __init__(self, number):
        self.n = number

    def getStrNum(self):
        return self.n ;

    def getASTCode(self):
        # print(f'in Num: {self}')
        return "(num " + self.n +")"


class Add(AE):
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


class Sub(AE):

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
