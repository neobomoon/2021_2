import sys
import WAE


class Parser:

    def parse(self, con_code):
        # split code
        con_code = self.split_con_code(con_code)
        # print(f'split_concrete_code: {con_code}')

        # num
        if len(con_code) == 1 and self.isint(con_code[0]):
            return WAE.Num( con_code[0] )

        # add
        elif con_code[0] == "+" and len( con_code ) == 3:
            return WAE.Add( self.parse( con_code[1] ), self.parse( con_code[2] ) )

        # sub
        elif con_code[0] == "-" and len( con_code ) == 3:
            return WAE.Sub( self.parse( con_code[1] ), self.parse( con_code[2] ) )

        # with
        if con_code[0] == "with" and len( con_code ) == 3:
            id_val = self.split_con_code(con_code[1])
            return WAE.With( id_val[0], self.parse(id_val[1]), self.parse(con_code[2]) )

        #id (대소문자 영어만 가능)
        elif self.checkId( con_code[0] ) and len( con_code ) == 1 :
            return WAE.Id( con_code[0] )
        else:
            print("wrong syntax!")
            sys.exit(1)

        return None

    def split_con_code(self, con_code):
        # print(con_code)
        # 양쪽이 {}가 아니면 에러, 숫자일 경우 빠진다
        if (con_code[0] != '{' and con_code[len(con_code) - 1] == '}') or (con_code[0] == '{' and con_code[len(con_code) - 1] != '}'):
            print("wrong input argument (split_con_code)")
            sys.exit(1)

        sub_exp = []
        buf = ""
        count = 0
        for i in range(0, len(con_code), 1):
            if count == 0:
                if (i == 0 and con_code[i] == "{") or (i == len(con_code) - 1 and con_code[len(con_code) - 1] == "}"):
                    continue

                if con_code[i] == ' ':
                    sub_exp.append(buf)
                    buf = ""
                    continue
                buf += con_code[i]

                if con_code[i] == "{":
                    count += 1 
            elif count > 0:
                buf += con_code[i]

                if con_code[i] == "{":
                    count += 1 
                if con_code[i] == "}":
                    count -= 1 
        sub_exp.append(buf)
        # print(f'after: {sub_exp}, {con_code}')
    
        return sub_exp

    def isint(self, num):
        temp = num.replace("-", "", 1)
        if temp.isdigit():
            return True
        else:
            return False
    
    def checkId(self, con_code):
        for i in range(0, len(con_code)):
            if (ord(con_code[i]) >= 65 and ord(con_code[i]) <= 90) or (ord(con_code[i]) >= 97 and ord(con_code[i]) <= 122):
                continue
            else:
                return False
        
        return True