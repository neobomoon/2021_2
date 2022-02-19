

class FAEValue:
    """super class"""
    astCode = ""
    def getValues(self):
        if isinstance(self, NumV):
            result = self.getValue()
        if isinstance(self, ClosureV):
            result = self.getValue()
        
        return result


class NumV(FAEValue):
    def __init__(self, n):
        self.n = n
    
    def getStrNum(self):
        return self.n
    
    def getValue(self):
        return "(numV " + self.n + ")"

class ClosureV(FAEValue):
    def __init__(self, param, body, ds):
        self.param = param
        self.body = body
        self.ds = ds
    
    def getParam(self):
        return self.param

    def getBody(self):
        return self.body

    def getDs(self):
        return self.ds
    
    def getValue(self):
        return "(closureV " + "'" + self.param + " " + self.body.getASTCode() + " " + self.ds.getCache() + ")"
