
class LFAE:
    """super  class"""
    astCode = ""
    def getASTCodes(self):
        if isinstance(self, Num):
            astCode = self.getASTCode()

        if isinstance(self, Add):
            astCode = self.getASTCode()

        if isinstance(self, Sub):
            astCode = self.getASTCode()
        
        if isinstance(self, Id):
            astCode = self.getASTCode()

        if isinstance(self, Fun):
            astCode = self.getASTCode()

        if isinstance(self, App):
            astCode = self.getASTCode()

        return astCode

class Num(LFAE):
    def __init__(self, number):
        self.n = number

    def getStrNum(self):
        return self.n 

    def getASTCode(self):
        # print(f'in Num: {self}')
        return "(num " + self.n + ")"


class Add(LFAE):
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


class Sub(LFAE):

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


class Id(LFAE):
    def __init__(self, name):
        self.name = name

    def getId(self):
        return self.name

    def getASTCode(self):
        # print(f'in Num: {self}')
        return "(id '" + self.name + ")"


class Fun(LFAE):
    def __init__(self, param, body):
        self.param = param
        self.body = body

    def getParam(self):
        return self.param

    def getBody(self):
        return self.body
    
    def getASTCode(self):
        return "(fun " + "'" + self.param + " " + self.body.getASTCode() + ")"
    
class App(LFAE):
    def __init__(self, ftn, arg):
        self.ftn = ftn
        self.arg = arg
    
    def getFtn(self):
        return self.ftn
    
    def getArg(self):
        return self.arg
    
    def getASTCode(self):
        return "(app " + self.ftn.getASTCode() + " " + self.arg.getASTCode() + ")"

