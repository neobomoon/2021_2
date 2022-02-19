import WAE
import sys

class Interpreter:
    def interp(self, ast):
        if isinstance(ast, WAE.Num):
            # print(f'interp add : {ast}')
            return ast.getStrNum()
        
        if isinstance(ast, WAE.Add):
            # print(f'interp add : {ast}')
            result =  self.atoi( self.interp(ast.getLhs()) ) + self.atoi( self.interp(ast.getRhs()) )
            return str(result)
        
        if isinstance(ast, WAE.Sub):
            # print(f'interp add : {ast}')
            result =  self.atoi( self.interp(ast.getLhs()) ) - self.atoi( self.interp(ast.getRhs()) )
            return str(result)
        
        if isinstance(ast, WAE.With):
            result = self.atoi( self.interp( self.subst( ast.getBody(), ast.getName(), self.interp( ast.getNamed_expr() ) ) ) )
            return str(result)

        if isinstance(ast, WAE.Id):
            print("Free identifier")
            sys.exit(1)

    def atoi(self, num):
        if "-" in num :
            num = num.replace("-", "")
            return -1 * int(num)
        else:
            return int(num)

    def subst(self, wae, idtf, val):
        if isinstance(wae, WAE.Num):
            return wae
        
        if isinstance(wae, WAE.Add):
            return WAE.Add( self.subst(wae.getLhs(), idtf, val), self.subst(wae.getRhs(), idtf, val) )
        
        if isinstance(wae, WAE.Sub):
            return WAE.Sub( self.subst(wae.getLhs(), idtf, val), self.subst(wae.getRhs(), idtf, val) )
        
        if isinstance(wae, WAE.With):
            if wae.getName() == idtf:
                body = wae.getBody()
            else:
                body = self.subst(wae.getBody(), idtf, val)
            return WAE.With( wae.getName(), self.subst(wae.getNamed_expr(), idtf, val), body )

        if isinstance(wae, WAE.Id):
            if wae.getId() == idtf:
                return WAE.Num(val)
            else:
                return wae

