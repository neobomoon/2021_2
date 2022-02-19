

class LFAEDSValue:
    """super class"""
    astCode = ""
    def getValues(self):
        if isinstance(self, NumV):
            result = self.getValue()
        if isinstance(self, ClosureV):
            result = self.getValue()
        
        return result


class NumV(LFAEDSValue):
    def __init__(self, n):
        self.n = n
    
    def getStrNum(self):
        return self.n
    
    def getValue(self):
        return "(numV " + self.n + ")"

class ClosureV(LFAEDSValue):
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

class exprV(LFAEDSValue):
    def __init__(self, expr, ds, box):
        self.expr = expr
        self.ds =  ds
        self.box = box

    def getExpr(self):
        return self.expr
    
    def getDs(self):
        return self.ds

    def getBox(self):
        return self.box

    def getValue(self):
        if not self.box:
            box = '#&#f'
        else:
            box = "#&" + self.box
        return "(exprV " + self.expr.getASTCode() + " " + self.ds.getCache() + " " + box + ")"
