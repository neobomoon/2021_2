import LFAEDS
import sys
import LFAEDSValue
import DefrdSub

class Interpreter:
    def interp( self, ast, ds ):
        if isinstance( ast, LFAEDS.Num ):
            # print(f'interp add : {ast}')
            return LFAEDSValue.NumV( ast.getStrNum() )
        
        if isinstance( ast, LFAEDS.Add ):
            # print(f'interp add : {ast}')
            result = self.atoi( self.interp(ast.getLhs(), ds ) ) + self.atoi( self.interp(ast.getRhs(), ds) )
            return LFAEDSValue.NumV( str( result ) )
        
        if isinstance( ast, LFAEDS.Sub ):
            # print(f'interp add : {ast}')
            result = self.atoi( self.interp(ast.getLhs(), ds ) ) - self.atoi( self.interp(ast.getRhs(), ds ) )
            return LFAEDSValue.NumV( str( result ) )

        if isinstance( ast, LFAEDS.Id ):
            return self.lookup(ast.getId(), ds, ds)

        if isinstance( ast, LFAEDS.Fun ):
            return LFAEDSValue.ClosureV( ast.getParam(), ast.getBody(), ds )

        if isinstance( ast, LFAEDS.Dsfun ):
            return LFAEDSValue.ClosureV( ast.getParam(), ast.getBody(), ds )

        if isinstance(ast, LFAEDS.App):
            f_val = self.strict( self.interp( ast.getFtn(), ds ) )
            a_val = LFAEDSValue.exprV( ast.getArg(), ds, False )

            return self.interp( f_val.getBody(), DefrdSub.aSub( f_val.getParam(), a_val, f_val.getDs() ) )

    def strict( self, v ):
        if isinstance( v, LFAEDSValue.exprV ):
            if not v.getBox():
                v = self.strict( self.interp( v.getExpr(), v.getDs() ) )
                v.box = v
                return v
            else:
                return v.getBox()
        else:
            return v


    def lookup( self, name, ds, latest_ds ):
        if isinstance( ds, DefrdSub.mtSub ):
            print("Free identifier") # lookup 추가하기
            sys.exit( 1 )

        if isinstance( ds, DefrdSub.aSub ):
            if ds.getName() == name:
                exprV = ds.getValue()
                if isinstance(exprV.getExpr(), LFAEDS.Dsfun):
                    exprV.ds = latest_ds
                    return self.strict( exprV )
                else:
                    return self.strict( ds.getValue() )
            else:
                return self.lookup( name, ds.getDs(), latest_ds)


    def atoi(self, num):
        if isinstance(num, LFAEDSValue.NumV):
            return self.atoi(num.getStrNum())
        else:
            if "-" in num :
                num = num.replace("-", "")
                return -1 * int(num)
            else:
                return int(num)
