import FAE
import sys
import FAEValue
import DefrdSub

class Interpreter:
    def interp(self, ast, ds):
        if isinstance( ast, FAE.Num ):
            # print(f'interp add : {ast}')
            return FAEValue.NumV( ast.getStrNum() )
        
        if isinstance( ast, FAE.Add ):
            # print(f'interp add : {ast}')
            result =  self.atoi( self.interp(ast.getLhs(), ds ) ) + self.atoi( self.interp(ast.getRhs(), ds) )
            return FAEValue.NumV( str( result ) )
        
        if isinstance( ast, FAE.Sub ):
            # print(f'interp add : {ast}')
            result = self.atoi( self.interp(ast.getLhs(), ds ) ) - self.atoi( self.interp(ast.getRhs(), ds ) )
            return FAEValue.NumV( str( result ) )

        if isinstance( ast, FAE.Id ):
            return self.lookup(ast.getId(), ds)

        if isinstance( ast, FAE.Fun ):
            return FAEValue.ClosureV( ast.getParam(), ast.getBody(), ds )

        if isinstance(ast, FAE.App):
            f_val = self.interp( ast.getFtn(), ds )
            a_val = self.interp( ast.getArg(), ds )

            return self.interp( f_val.getBody(), DefrdSub.aSub( f_val.getParam(), a_val, f_val.getDs() ) )


    def lookup( self, name, ds ):
        if isinstance( ds, DefrdSub.mtSub ):
            print("Free identifier") # lookup 추가하기
            sys.exit(1)

        if isinstance( ds, DefrdSub.aSub ):
            if ds.getName() == name:
                return ds.getValue()
            else:
                return self.lookup( name, ds.getDs() )


    def atoi(self, num):
        if isinstance(num, FAEValue.NumV):
            return self.atoi(num.getStrNum())
        else:
            if "-" in num :
                num = num.replace("-", "")
                return -1 * int(num)
            else:
                return int(num)

    def subst(self, fae, idtf, val):
        if isinstance(fae, FAE.Num):
            return fae
        
        if isinstance(fae, FAE.Add):
            return FAE.Add( self.subst(fae.getLhs(), idtf, val), self.subst(fae.getRhs(), idtf, val) )
        
        if isinstance(fae, FAE.Sub):
            return FAE.Sub( self.subst(fae.getLhs(), idtf, val), self.subst(fae.getRhs(), idtf, val) )
        
        if isinstance(fae, FAE.With):
            if fae.getName() == idtf:
                body = fae.getBody()
            else:
                body = self.subst(fae.getBody(), idtf, val)
            return FAE.With( fae.getName(), self.subst(fae.getNamed_expr(), idtf, val), body )

        if isinstance(fae, FAE.Id):
            if fae.getId() == idtf:
                return FAE.Num(val)
            else:
                return fae

