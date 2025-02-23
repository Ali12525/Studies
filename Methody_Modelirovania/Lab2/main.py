a = 0
x = 0
                
def podprogram():
    global a
    print("a3/y4")
    while(1):
        x = int(input("Введите х4 (0 или 1): "))
        if(x == 0):
                print("a13/y14")
                a = 13
        elif(x == 1):
                print("ak/yk")
                return
        elif(a == 13):
            x = int(input("Введите х5 (0 или 1): "))
            if(x == 0):
                print("a14/y15")
                a = 14
                x = int(input("Введите х6 (0 или 1): "))
                if(x == 0):
                    print("a15/y16")
                    a = 15
                    print("a13/y14")
                    a = 13
                else:
                    print("a16/y17")
                    a = 16
            else:
                print("a16/y17")
                a = 16
                x = int(input("Введите х7 (0 или 1): "))
                if(x == 0):
                    a = 17
                    print("a17/y18")
                    x = int(input("Введите х7 (0 или 1): "))
                    if(x == 0):
                        a = 17
                        print("a17/y18")
                    else:
                        return
                else:
                    return
        
    while(1):
        if(a == 3):
            x = int(input("Введите х4 (0 или 1): "))
            if(x == 0):
                print("a13/y14")
                a = 13
            else:
                print("ak/yk")
                return
        if(a == 13):
            x = int(input("Введите х5 (0 или 1): "))
            if(x == 0):
                print("a14/y15")
                a = 14
                x = int(input("Введите х6 (0 или 1): "))
                if(x == 0):
                    print("a15/y16")
                    a = 15
                    print("a13/y14")
                    a = 13
                else:
                    print("a16/y17")
                    a = 16
            else:
                print("a16/y17")
                a = 16
                x = int(input("Введите х7 (0 или 1): "))
                if(x == 0):
                    a = 17
                    print("a17/y18")
                    x = int(input("Введите х7 (0 или 1): "))
                    if(x == 0):
                        a = 17
                        print("a17/y18")
                    else:
                        return
                else:
                    return

while(1):
    if(a == 0):
        print("a0/y1")
        a = 1
    elif(a == 1):
        print("a1/y2")
        a = 2
    elif(a == 2):
        print("a2/y3")
        a = 3
    elif(a == 3):
        print("a3/y4")
        podprogram()
        print("a4/y5")
        a = 4
    elif(a == 4):
        x = int(input("Введите х1 (0 или 1): "))
        if(x == 0):
            a = 6
            print("a6/y7")
        else:
            a = 7
            print("a7/y8")
    elif(a == 6):
            x = int(input("Введите х2 (0 или 1): "))
            if(x == 0):
                a = 5
                print("a5/y6")
            else:
                a = 9
                print("a9/y10")
    elif(a == 5):
        x = int(input("Введите х1 (0 или 1): "))
        if(x == 0):
            a = 6
            print("a6/y7")
        else:
            a = 7
            print("a7/y8")
    elif(a == 9):
        a = 2
        print("a2/y3")
    elif(a == 7):
        a = 8
    elif(a == 8):
        a = 10
        print("a8/y9")
    elif(a == 10):
        print("a10/y11")
        print("a3/y4")
        podprogram()
        a = 11
    elif(a == 11):
        print("a11/y12")
        a = 12
    elif(a == 12):
        print("a12/y13")
        print("a3/y4")
        podprogram()
        break
        
