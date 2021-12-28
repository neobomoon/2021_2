import LFAE
import sys
import LFAEValue
import DefrdSub

class Interpreter:
    def interp( self, ast, ds ):
        if isinstance( ast, LFAE.Num ):
            # print(f'interp add : {ast}')
            return LFAEValue.NumV( ast.getStrNum() )
        
        if isinstance( ast, LFAE.Add ):
            # print(f'interp add : {ast}')
            result = self.atoi( self.strict( self.interp(ast.getLhs(), ds ) ) ) + self.atoi( self.strict( self.interp(ast.getRhs(), ds) ) )
            return LFAEValue.NumV( str( result ) )
        
        if isinstance( ast, LFAE.Sub ):
            # print(f'interp add : {ast}')
            result = self.atoi( self.strict( self.interp(ast.getLhs(), ds ) ) ) - self.atoi( self.strict( self.interp(ast.getRhs(), ds ) ) )
            return LFAEValue.NumV( str( result ) )

        if isinstance( ast, LFAE.Id ):
            return self.lookup(ast.getId(), ds)

        if isinstance( ast, LFAE.Fun ):
            return LFAEValue.ClosureV( ast.getParam(), ast.getBody(), ds )

        if isinstance(ast, LFAE.App):
            f_val = self.strict( self.interp( ast.getFtn(), ds ) )
            a_val = LFAEValue.exprV( ast.getArg(), ds, False )

            return self.interp( f_val.getBody(), DefrdSub.aSub( f_val.getParam(), a_val, f_val.getDs() ) )

    def strict( self, v ):
        if isinstance( v, LFAEValue.exprV ):
            if not v.getBox():
                v = self.strict( self.interp( v.getExpr(), v.getDs() ) )
                v.box = v
                return v
            else:
                return v.getBox()
        else:
            return v


    def lookup( self, name, ds ):
        if isinstance( ds, DefrdSub.mtSub ):
            print("Free identifier") # lookup 추가하기
            sys.exit( 1 )

        if isinstance( ds, DefrdSub.aSub ):
            if ds.getName() == name:
                return self.strict( ds.getValue() )
            else:
                return self.lookup( name, ds.getDs() )


    def atoi(self, num):
        if isinstance(num, LFAEValue.NumV):
            return self.atoi(num.getStrNum())
        else:
            if "-" in num :
                num = num.replace("-", "")
                return -1 * int(num)
            else:
                return int(num)

    def subst(self, lfae, idtf, val):
        if isinstance(lfae, LFAE.Num):
            return lfae
        
        if isinstance(lfae, LFAE.Add):
            return LFAE.Add( self.subst(lfae.getLhs(), idtf, val), self.subst(lfae.getRhs(), idtf, val) )
        
        if isinstance(lfae, LFAE.Sub):
            return LFAE.Sub( self.subst(lfae.getLhs(), idtf, val), self.subst(lfae.getRhs(), idtf, val) )
        
        if isinstance(lfae, LFAE.With):
            if lfae.getName() == idtf:
                body = lfae.getBody()
            else:
                body = self.subst(lfae.getBody(), idtf, val)
            return LFAE.With( lfae.getName(), self.subst(lfae.getNamed_expr(), idtf, val), body )

        if isinstance(lfae, LFAE.Id):
            if lfae.getId() == idtf:
                return LFAE.Num(val)
            else:
                return lfae

