import sys
import AE


class Parser:

    def parse(self, con_code):
        # split code
        con_code = self.split_con_code(con_code)
        # print(f'split_concrete_code: {con_code}')

        # num
        if len(con_code) == 1 and self.isint(con_code[0]):
            return AE.Num( con_code[0] )

        # add
        if con_code[0] == "+":
            # lhs
            return AE.Add( self.parse(con_code[1]), self.parse(con_code[2]) )

        # sub
        if con_code[0] == "-":
            # lhs
            return AE.Sub( self.parse(con_code[1]), self.parse(con_code[2]) )

        return None

    def split_con_code(self, con_code):
        # print(con_code)
        # 양쪽이 {}가 아니면 에러, 숫자일 경우 빠진다
        if (con_code[0] != '{' and con_code[len(con_code) - 1] == '}') or (con_code[0] == '{' and con_code[len(con_code) - 1] != '}'):
            print("wrong input argument (split_con_code)")
            sys.exit(1)

        if len(con_code) == 1:
            return [con_code]
        
        sub_exp = []
        buf = ""
        count = 0
        for i in range(1, len(con_code) - 1, 1):
            if count == 0:
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

        return sub_exp

    def isint(self, num):
        temp = num.replace("-", "", 1)
        if temp.isdigit():
            return True
        else:
            return False