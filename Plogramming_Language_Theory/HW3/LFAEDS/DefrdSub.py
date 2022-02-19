class DefrdSub:
    """super class"""


class mtSub(DefrdSub):
    def __init__(self):
        self.empty = None

    def getMtSub(self):
        return self.empty
    
    def getCache(self):
        return "(mtSub)"

class aSub(DefrdSub):
    def __init__(self, name, value, ds):
        self.name = name
        self.value = value
        self.ds = ds

    def getName(self):
        return self.name 
    
    def getValue(self):
        return self.value
    
    def getDs(self):
        return self.ds
    
    def getCache(self):
        return "(aSub " + "'" + self.name + " " + self.value.getValue() + " " + self.ds.getCache() + ")"
        


    
