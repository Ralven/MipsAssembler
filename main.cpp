#include <stdio.h>
#include <ctype.h>
#include <memory.h>
#include <stdlib.h>

typedef struct{
    int address;
    char type;
    char * name;
    int op;
    int rs;
    int rt;
    int rd;
    int shamt;
    int funct;
    int targaddr;
    int immed;
    int exaddr;
    char label[32];
    char bin[32];
}instruction;

typedef struct{
    int address;
    char type;
    char * name;
    int op[6];
    int rs[5];
    int rt[5];
    int rd[5];
    int shamt[5];
    int funct[6];
    int immed[16];
    int targaddr[26];
    int exaddr[32];
    char label[32];
    char bin[32];
}bin_Inst;

typedef struct{
    int address;
    char instr[256];
}instructionList;

typedef struct{
    int address;
    char name[32];
}Labels;

long bin2dec(bin_Inst bin){
    long long sum = 0;
    long long plus = 1;
    int counter = 0;
    int i = 30;
    for(;i >= 0;i--)
    {
        if (bin.bin[i] == '\001'){

            counter = 31 - i;
            while(counter != 0){
                plus = plus*2;
                counter--;
            }
            sum = plus+sum;
            plus = 1;
        }
    }
    if(bin.bin[31] == '\001'){
        sum++;
    }
    return sum;
}

void convertHelper(int instr, int bin_instr[], int size){
    int negative = 0;
    if(instr < 0){
        instr = instr * -1;
        negative = 1;
    }
    int j;
    int i = size-1;
    while (instr != 0) {
        j = instr % 2;
        bin_instr[i] = j;
        i--;
        instr = instr / 2;
    }
    if(negative == 1){
        int k = 0;
        for(;k < size-1;k++){
            if(bin_instr[k] == 1){
                bin_instr[k] = 0;
            }else if(bin_instr[k] == 0){
                bin_instr[k] = 1;
            }
        }
    }


}

bin_Inst convertToBin(instruction instruction){
    bin_Inst tempInst;
    int i = 0;
    for(; i < 32;i++)
    {
        tempInst.exaddr[i] = 0;
        tempInst.bin[i] = 0;
        if( i <= 28)
        {
            tempInst.targaddr[i] = 0;
        }
        if(i <= 16)
        {
            tempInst.immed[i]= 0;
        }
        if(i<=5) {
            tempInst.op[i] = 0;
            tempInst.funct[i] = 0;
        }
        if(i<=4){
            tempInst.rd[i] = 0;
            tempInst.rs[i] = 0;
            tempInst.rt[i] = 0;
            tempInst.shamt[i] = 0;
        }
    }
    tempInst.address = instruction.address;
    if(instruction.type == 'r'){
        tempInst.type = 'r';
        convertHelper(instruction.op,tempInst.op, (sizeof(tempInst.op)/ sizeof(tempInst.op[0])));
        convertHelper(instruction.rs,tempInst.rs,(sizeof(tempInst.rs)/ sizeof(tempInst.rs[0])));
        convertHelper(instruction.rt,tempInst.rt,(sizeof(tempInst.rt)/ sizeof(tempInst.rt[0])));
        convertHelper(instruction.rd,tempInst.rd,(sizeof(tempInst.rd)/ sizeof(tempInst.rd[0])));
        convertHelper(instruction.shamt,tempInst.shamt,(sizeof(tempInst.shamt)/ sizeof(tempInst.shamt[0])));
        convertHelper(instruction.funct,tempInst.funct,(sizeof(tempInst.funct)/ sizeof(tempInst.funct[0])));
    }
    else if(instruction.type == 'i'){
        tempInst.type = 'i';
        convertHelper(instruction.op,tempInst.op, (sizeof(tempInst.op)/ sizeof(tempInst.op[0])));
        convertHelper(instruction.rs,tempInst.rs,(sizeof(tempInst.rs)/ sizeof(tempInst.rs[0])));
        convertHelper(instruction.rt,tempInst.rt,(sizeof(tempInst.rt)/ sizeof(tempInst.rt[0])));
        convertHelper(instruction.exaddr,tempInst.exaddr,(sizeof(tempInst.exaddr)/ sizeof(tempInst.exaddr[0])));
        convertHelper(instruction.immed,tempInst.immed,(sizeof(tempInst.immed)/ sizeof(tempInst.immed[0])));
    }
    else if(instruction.type == 'j'){
        tempInst.type = 'j';
        convertHelper(instruction.op,tempInst.op,(sizeof(tempInst.op)/ sizeof(tempInst.op[0])));
        convertHelper(instruction.targaddr,tempInst.targaddr,(sizeof(tempInst.targaddr)/ sizeof(tempInst.targaddr[0])));
    }
    return tempInst;
};

bin_Inst buildBinInst(instruction inst){
    bin_Inst tempInst;
    tempInst.address = inst.address;
    int n = 0;
    for(; n < 32;n++)
    {
        tempInst.bin[n] = '0';
        tempInst.exaddr[n] = '0';
        tempInst.label[n] = '0';
        if( n <= 28)
        {
            tempInst.targaddr[n] = '0';
        }
        if(n <= 16)
        {
            tempInst.immed[n]= '0';
        }
        if(n<=5) {
            tempInst.op[n] = '0';
            tempInst.funct[n] = '0';
        }
        if(n<=4){
            tempInst.rd[n] = '0';
            tempInst.rs[n] = '0';
            tempInst.rt[n] = '0';
            tempInst.shamt[n] = '0';
        }
    }
    tempInst = convertToBin(inst);
    int k = 0;
    int r = 0;
    int i = 0;
    for(; i <= 31; i++){
        if(tempInst.type == 'r') {
            if (i <= 5) {
                if (k > 5) {
                    k = 0;
                }
                tempInst.bin[i] = tempInst.op[k];
                k++;
            } else if (i <= 10) {
                if (k > 4) {
                    k = 0;
                }
                tempInst.bin[i] = tempInst.rs[k];
                k++;
            } else if (i <= 15) {
                if (k > 4) {
                    k = 0;
                }
                tempInst.bin[i] = tempInst.rt[k];
                k++;
            } else if (i <= 20) {
                if (k > 4) {
                    k = 0;
                }
                tempInst.bin[i] = tempInst.rd[k];
                k++;
            } else if (i <= 25) {
                if (k > 4) {
                    k = 0;
                }
                tempInst.bin[i] = tempInst.shamt[k];
                k++;
                if (k >= 5) {
                    k = 0;
                }
            } else if (i < 32) {
                tempInst.bin[i] = tempInst.funct[r];
                r++;
            }
        }else if(tempInst.type == 'i'){

            if (i <= 5) {
                if (k > 5) {
                    k = 0;
                }
                tempInst.bin[i] = tempInst.op[k];
                k++;
            } else if (i <= 10) {
                if (k > 4) {
                    k = 0;
                }
                tempInst.bin[i] = tempInst.rs[k];
                k++;
            } else if (i <= 15) {
                if (k > 4) {
                    k = 0;
                }
                tempInst.bin[i] = tempInst.rt[k];
                k++;
            }else if (i < 32) {
                if(strcmp(inst.name,"lui") == 0) {
                    int tempRt[5] = {0,0,0,0,1};
                    int count = 0;
                    int boolean = 0;
                    if (tempInst.rt[4] == 1) {
                        while (tempInst.rt[count] == 0) {
                            if (tempInst.rt[count] == tempRt[count]) {
                                boolean = 1;
                            } else {
                                boolean = 0;
                            }
                            count++;
                        }
                        if (boolean == 1) {
                            tempInst.bin[i] = tempInst.exaddr[i - 16];
                        }
                    }
                }else if(strcmp(inst.name,"ori") == 0){
                    int tempRs[5] = {0,0,0,0,1};
                    int count= 0;
                    int boolean = 0;
                    if(tempInst.rs[4] ==1) {
                        while (tempInst.rs[count] == 0) {
                            if (tempInst.rs[count] == tempRs[count]) {
                                boolean = 1;
                            } else {
                                boolean = 0;
                            }
                            count++;
                        }
                        if (boolean == 1) {
                            tempInst.bin[i] = tempInst.exaddr[i];
                        }
                    }
                }else{
                    tempInst.bin[i] = tempInst.immed[r];
                    r++;
                }
            }
        }else if(tempInst.type == 'j'){
            if (i <= 5) {
                if (k > 5) {
                    k = 0;
                }
                tempInst.bin[i] = tempInst.op[k];
                k++;
            }else if (i < 32) {
                tempInst.bin[i] = tempInst.targaddr[r];
                r++;
            }
        }
    }
    printf("0x%.8X: 0x%.08lX\n",tempInst.address,bin2dec(tempInst));
    return tempInst;
}

instruction buildRinst(char instr[], int instrLength,int address){
    instruction tempInstr;
    int checker = 0;
    char shift[32];
    int shiftdigit = 0;
    int k = 0;
    for(; k < instrLength; k ++) {
        /**************Checking which R Instruction and Beginning to Build it****************/
        if (instr[k] == 'a') {
            if (instr[k + 1] == 'd')
                if (instr[k + 2] == 'd')
                    if (instr[k + 3] != 'i') {
                        tempInstr.address = address;
                        tempInstr.type = 'r';
                        tempInstr.name = "add";
                        tempInstr.rs = 0;
                        tempInstr.rt = 0;
                        tempInstr.rd = 0;
                        tempInstr.op = 0;
                        tempInstr.shamt = 0;
                        tempInstr.funct = 32;
                    }
        }
        if (instr[k] == 'n') {
            if (instr[k + 1] == 'o')
                if (instr[k + 2] == 'r') {
                    tempInstr.address = address;
                    tempInstr.type = 'r';
                    tempInstr.name = "nor";
                    tempInstr.rs = 0;
                    tempInstr.rt = 0;
                    tempInstr.rd = 0;
                    tempInstr.op = 0;
                    tempInstr.shamt = 0;
                    tempInstr.funct = 39;
                }
        }
        if (instr[k] == 's') {
            if (instr[k + 1] == 'l')
                if (instr[k + 2] == 'l') {
                    tempInstr.address = address;
                    tempInstr.type = 'r';
                    tempInstr.name = "sll";
                    tempInstr.rs = 0;
                    tempInstr.rt = 0;
                    tempInstr.rd = 0;
                    tempInstr.op = 0;
                    tempInstr.shamt = 0;
                    tempInstr.funct = 0;
                }
        }
        /**************Checking what Registers are used and adding to Build it****************/

        if(instr[k] == '$'){
            if(instr[k+1] == 't' || instr[k+1] == 's')
            {
                int i = 0;
                for(; i <= 7; i++) {
                    if(atoi(&instr[k+2]) == i){
                        if(checker == 0){
                            if(strcmp(tempInstr.name, "add") == 0 || strcmp(tempInstr.name, "nor") == 0 || strcmp(tempInstr.name, "sll") == 0) {
                                if(instr[k+1] == 't') {
                                    tempInstr.rd = i + 8;
                                }else if(instr[k+1] == 's'){
                                    tempInstr.rd = i + 16;
                                }
                            }
                            checker++;
                            break;
                        }
                        else if(checker ==1) {
                            if (strcmp(tempInstr.name, "add") == 0 || strcmp(tempInstr.name, "nor") == 0) {
                                if (instr[k + 1] == 't') {
                                    tempInstr.rs = i + 8;
                                } else if (instr[k + 1] == 's') {
                                    tempInstr.rs = i + 16;
                                }
                            } else if (strcmp(tempInstr.name, "sll") == 0) {
                                if (instr[k + 1] == 't') {
                                    tempInstr.rt = i + 8;
                                } else if (instr[k + 1] == 's') {
                                    tempInstr.rt = i + 16;
                                }
                            }
                            checker++;
                            break;
                        }
                        else if(checker == 2){
                            if(instr[k+1] == 't') {
                                tempInstr.rt = i + 8;
                            }else if(instr[k+1] == 's'){
                                tempInstr.rt = i + 16;
                            }
                            checker++;
                            break;
                        }
                    }
                }
                k = k + 3;
            }
            else if(instr[k+1] == '0')
            {
                k = k + 2;
            }
        }
        if(instr[k] == ',' && strcmp(tempInstr.name,"sll") == 0) {
            if (checker == 2){
                while(k+1!=instrLength){
                    shift[shiftdigit] = instr[k+1];
                    k++;
                }
            }
            tempInstr.shamt = atoi(shift);
        }
    }
    return tempInstr;
};

instruction buildIinst(char instr[], int instrLength,int address, Labels labelList[], int sizeLabelList){
    instruction tempInstr;
    tempInstr.name = NULL;
    char immed[32];
    int immedDigit = 0;
    int k = 0;
    for(; k < instrLength; k ++) {
        /**************Checking which I Instruction and Beginning to Build it****************/
        if (instr[k] == 'a') {
            if (instr[k + 1] == 'd')
                if (instr[k + 2] == 'd')
                    if (instr[k + 3] == 'i') {
                        tempInstr.address = address;
                        tempInstr.type = 'i';
                        tempInstr.name = "addi";
                        tempInstr.rs = 0;
                        tempInstr.rt = 0;
                        tempInstr.rd = 0;
                        tempInstr.op = 8;
                        tempInstr.immed = 0;
                        tempInstr.shamt = 0;
                        tempInstr.funct = 0;
                        k = k + 4;
                    }
        }
        if (instr[k] == 'o') {
            if (instr[k + 1] == 'r')
                if (instr[k + 2] == 'i') {
                    tempInstr.address = address;
                    tempInstr.type = 'i';
                    tempInstr.name = "ori";
                    tempInstr.exaddr = 0;
                    tempInstr.rs = 0;
                    tempInstr.rt = 0;
                    tempInstr.rd = 0;
                    tempInstr.op = 13;
                    tempInstr.immed = 0;
                    tempInstr.shamt = 0;
                    tempInstr.funct = 0;
                    k = k + 3;
                }
        }
        if (instr[k] == 'l') {
            if (instr[k + 1] == 'u')
                if (instr[k + 2] == 'i') {
                    tempInstr.address = address;
                    tempInstr.type = 'i';
                    tempInstr.name = "lui";
                    tempInstr.exaddr = 0;
                    tempInstr.rs = 0;
                    tempInstr.rt = 0;
                    tempInstr.rd = 0;
                    tempInstr.op = 15;
                    tempInstr.immed = 0;
                    tempInstr.shamt = 0;
                    tempInstr.funct = 0;
                    k = k + 3;
                }
        }
        if (instr[k] == 's') {
            if (instr[k + 1] == 'w') {
                tempInstr.address = address;
                tempInstr.type = 'i';
                tempInstr.name = "sw";
                tempInstr.rs = 0;
                tempInstr.rt = 0;
                tempInstr.rd = 0;
                tempInstr.op = 43;
                tempInstr.immed = 0;
                tempInstr.shamt = 0;
                tempInstr.funct = 0;
                k = k + 2;
            }
        }
        if (instr[k] == 'l') {
            if (instr[k + 1] == 'w') {
                tempInstr.address = address;
                tempInstr.type = 'i';
                tempInstr.name = "lw";
                tempInstr.rs = 0;
                tempInstr.rt = 0;
                tempInstr.rd = 0;
                tempInstr.op = 35;
                tempInstr.immed = 0;
                tempInstr.shamt = 0;
                tempInstr.funct = 0;
                k = k + 2;
            }
        }
        if (instr[k] == 'b') {
            if (instr[k + 1] == 'n')
                if (instr[k + 2] == 'e') {
                    tempInstr.address = address;
                    tempInstr.type = 'i';
                    tempInstr.name = "bne";
                    tempInstr.rs = 0;
                    tempInstr.rt = 0;
                    tempInstr.rd = 0;
                    tempInstr.op = 5;
                    tempInstr.immed = 0;
                    tempInstr.shamt = 0;
                    tempInstr.funct = 0;
                    k = k + 3;
                }
        }
        /**************Checking what Registers are used and adding to Build it****************/
        if (instr[k] == '$' && (strcmp(tempInstr.name, "bne") != 0) && (strcmp(tempInstr.name, "lui") != 0 &&
                                                                        (strcmp(tempInstr.name, "ori") != 0))) {
            k++;
            if (instr[k] == 't' || instr[k] == 's') {
                k++;
                int i = 0;
                for (; i <= 7; i++) {
                    if (atoi(&instr[k]) == i) {
                        if (tempInstr.rt == 0) {
                            k--;
                            if (instr[k] == 't') {
                                tempInstr.rt = i + 8;
                                k++;
                                break;
                            } else if (instr[k] == 's') {
                                tempInstr.rt = i + 16;
                                k++;
                                break;
                            }
                            k++;
                        } else if (tempInstr.rs == 0) {
                            k--;
                            if (instr[k] == 't') {
                                tempInstr.rs = i + 8;
                                k++;
                                break;
                            } else if (instr[k] == 's') {
                                tempInstr.rs = i + 16;
                                k++;
                                break;
                            }
                            k++;
                        }
                    }
                }
                k++;
            } else if (instr[k] == '0') {
                k++;
            } else if (instr[k] == '1') {
                tempInstr.rt = 1;
            }
        } else if (isdigit(instr[k]) != 0) {
            while (isdigit(instr[k]) != 0) {
                immed[immedDigit] = instr[k];
                immedDigit++;
                if (!isdigit(instr[k + 1])) {
                    break;
                }
                k++;
            }
            tempInstr.immed = atoi(immed);
        } else if (instr[k] == '(') {
            k++;
            if (instr[k] == '$') {
                k++;
                if (instr[k] == 't' || instr[k] == 's') {
                    k++;
                    int i = 0;
                    for (; i <= 7; i++) {
                        if (atoi(&instr[k]) == i) {
                            if (tempInstr.rs == 0) {
                                k--;
                                if (instr[k] == 't') {
                                    tempInstr.rs = i + 8;
                                    k++;
                                    break;
                                } else if (instr[k] == 's') {
                                    tempInstr.rs = i + 16;
                                    k++;
                                    break;
                                }
                            }
                        }
                    }
                    k++;
                }
            }
        }
        if (tempInstr.name != NULL) {
            if (strcmp(tempInstr.name, "bne") == 0) {
                if (instr[k] == '$') {
                    k++;
                    if (instr[k] == 't' || instr[k] == 's') {
                        k++;
                        int i = 0;
                        for (; i <= 7; i++) {
                            if (atoi(&instr[k]) == i) {
                                if (tempInstr.rs == 0) {
                                    k--;
                                    if (instr[k] == 't') {
                                        tempInstr.rs = i + 8;
                                        k++;
                                        break;
                                    } else if (instr[k] == 's') {
                                        tempInstr.rs = i + 16;
                                        k++;
                                        break;
                                    }
                                    k++;
                                } else if (tempInstr.rt == 0) {
                                    k--;
                                    if (instr[k] == 't') {
                                        tempInstr.rt = i + 8;
                                        k++;
                                        break;
                                    } else if (instr[k] == 's') {
                                        tempInstr.rt = i + 16;
                                        k++;
                                        break;
                                    }
                                    k++;
                                }
                            }
                        }
                        k++;
                    }
                } else if (isalpha(instr[k]) != 0) {
                    int count = 0;
                    while (isalpha(instr[k]) != 0) {
                        tempInstr.label[count] = instr[k];
                        count++;
                        k++;
                    }
                    int sizeLabel = sizeof(&labelList[0].name) / sizeof(&labelList[0].name[0]);
                    int i = 0;
                    for (; i < sizeLabelList; i++) {
                        int equal = 0;
                        int j = 0;
                        while (labelList[i].name[j] != '\000') {
                            if (tempInstr.label[j] == labelList[i].name[j]) {
                                equal = 1;
                            } else {
                                equal = 0;
                                break;
                            }

                            j++;
                        }
                        if (equal == 1) {
                            tempInstr.immed = (labelList[i].address - tempInstr.address) / 4;
                        }
                    }
                }
            }
        }
        if (tempInstr.name != NULL) {
            if (strcmp(tempInstr.name, "lui") == 0) {
                if (instr[k] == '$') {
                    k++;
                    if (instr[k] == 't' || instr[k] == 's') {
                        k++;
                        int i = 0;
                        for (; i <= 7; i++) {
                            if (atoi(&instr[k]) == i) {
                                if (tempInstr.rs == 0) {
                                    k--;
                                    if (instr[k] == 't') {
                                        tempInstr.rs = i + 8;
                                        k++;
                                        break;
                                    } else if (instr[k] == 's') {
                                        tempInstr.rs = i + 16;
                                        k++;
                                        break;
                                    }
                                    k++;
                                } else if (tempInstr.rt == 0) {
                                    k--;
                                    if (instr[k] == 't') {
                                        tempInstr.rt = i + 8;
                                        k++;
                                        break;
                                    } else if (instr[k] == 's') {
                                        tempInstr.rt = i + 16;
                                        k++;
                                        break;
                                    }
                                    k++;
                                }
                            }
                        }
                        k++;
                    } else if (instr[k] == '0') {
                        k++;
                    } else if (instr[k] == '1') {
                        tempInstr.rt = 1;
                    }
                }
                if (isalpha(instr[k]) != 0 || ispunct(instr[k]) != 0) {
                    if(instr[k] != ',') {
                        int count = 0;
                        while ((isalpha(instr[k]) != 0) || (ispunct(instr[k]) != 0)) {
                            tempInstr.label[count] = instr[k];
                            count++;
                            k++;
                        }
                        int sizeLabel = sizeof(&labelList[0].name) / sizeof(&labelList[0].name[0]);
                        int i = 0;
                        for (; i < sizeLabelList; i++) {
                            int equal = 0;
                            int j = 0;
                            while (labelList[i].name[j] != '\000') {
                                if (tempInstr.label[j] == labelList[i].name[j]) {
                                    equal = 1;
                                    j++;
                                }
                                else{
                                    equal = 0;
                                    j++;
                                    break;
                                }
                            }
                            if(equal == 1){
                                tempInstr.exaddr = labelList[i].address;
                                break;
                            }
                        }
                    }
                }

            }
        }
        if (tempInstr.name != NULL) {
            if (strcmp(tempInstr.name, "ori") == 0) {
                if (instr[k] == '$') {
                    k++;
                    if (instr[k] == 't' || instr[k] == 's') {
                        k++;
                        int i = 0;
                        for (; i <= 7; i++) {
                            if (atoi(&instr[k]) == i) {
                                if (tempInstr.rt == 0) {
                                    k--;
                                    if (instr[k] == 't') {
                                        tempInstr.rt = i + 8;
                                        k++;
                                        break;
                                    } else if (instr[k] == 's') {
                                        tempInstr.rt = i + 16;
                                        k++;
                                        break;
                                    }
                                    k++;
                                } else if (tempInstr.rs == 0) {
                                    k--;
                                    if (instr[k] == 't') {
                                        tempInstr.rs = i + 8;
                                        k++;
                                        break;
                                    } else if (instr[k] == 's') {
                                        tempInstr.rs = i + 16;
                                        k++;
                                        break;
                                    }
                                    k++;
                                }
                            }
                        }
                        k++;
                    } else if (instr[k] == '0') {
                        k++;
                    } else if (instr[k] == '1') {
                        tempInstr.rs = 1;
                    }
                }
                if (isalpha(instr[k]) != 0 || ispunct(instr[k]) != 0) {
                    if(instr[k] != ',') {
                        int count = 0;
                        while ((isalpha(instr[k]) != 0) || (ispunct(instr[k]) != 0)) {
                            tempInstr.label[count] = instr[k];
                            count++;
                            k++;
                        }
                        int sizeLabel = sizeof(&labelList[0].name) / sizeof(&labelList[0].name[0]);
                        int i = 0;
                        for (; i < sizeLabelList; i++) {
                            int equal = 0;
                            int j = 0;
                            while (labelList[i].name[j] != '\000') {
                                if (tempInstr.label[j] == labelList[i].name[j]) {
                                    equal = 1;
                                }
                                else{
                                    equal = 0;
                                    break;
                                }

                                j++;
                            }
                            if(equal == 1){
                                tempInstr.exaddr = labelList[i].address;
                            }
                        }
                    }
                }

            }
        }
    }

    return tempInstr;
};

instruction buildJinst(char instr[], int instrLength,int address, Labels labelList[], int sizeLabelList){
    instruction tempInstr;
    int k = 0;
    for(; k < instrLength; k ++) {
        if(instr[k] == 'j')
        {
            k++;
            tempInstr.op = 2;
            tempInstr.address = address;
            tempInstr.type = 'j';
            tempInstr.name = "j";
            tempInstr.rs = 0;
            tempInstr.rt = 0;
            tempInstr.rd = 0;
            tempInstr.immed = 0;
            tempInstr.shamt = 0;
            tempInstr.funct = 0;
            tempInstr.targaddr = 0;

        }
        else if (isalpha(instr[k]) != 0) {
            int count = 0;
            while (isalpha(instr[k]) != 0) {
                tempInstr.label[count] = instr[k];
                count++;
                k++;
            }
            int sizeLabel = sizeof(&labelList[0].name) / sizeof(&labelList[0].name[0]);
            int i = 0;
            for (; i < sizeLabelList; i++) {
                int equal = 0;
                int j = 0;
                while (labelList[i].name[j] != '\000') {
                    if (tempInstr.label[j] == labelList[i].name[j]) {
                        equal = 1;
                    }
                    else{
                        equal = 0;
                        break;
                    }

                    j++;
                }
                if(equal == 1){
                    tempInstr.targaddr = labelList[i].address/4;
                }
            }
        }
    }
    return tempInstr;
};

char checkType(char instr[], int instrLength){
    int k = 0;
    for(; k < instrLength; k ++)
    {
        /**************Checking for R****************/
        if(instr[k] == 'a') {
            if (instr[k + 1] == 'd')
                if (instr[k + 2] == 'd')
                    if (instr[k + 3] != 'i'){
                        return 'r';
                    }
        }
        if(instr[k] == 'n') {
            if (instr[k + 1] == 'o')
                if (instr[k + 2] == 'r'){
                    return 'r';
                }
        }
        if(instr[k] == 's') {
            if (instr[k + 1] == 'l')
                if (instr[k + 2] == 'l'){
                    return 'r';
                }
        }
        /**************************************/
        /**************Checking for I************/
        if(instr[k] == 'a') {
            if (instr[k + 1] == 'd')
                if (instr[k + 2] == 'd')
                    if (instr[k + 3] == 'i'){
                        return 'i';
                    }
        }
        if(instr[k] == 'o') {
            if (instr[k + 1] == 'r')
                if (instr[k + 2] == 'i'){
                    return 'i';
                }
        }
        if(instr[k] == 'l') {
            if (instr[k + 1] == 'u')
                if (instr[k + 2] == 'i'){
                    return 'i';
                }
        }
        if(instr[k] == 's') {
            if (instr[k + 1] == 'w'){
                return 'i';
            }
        }
        if(instr[k] == 'l') {
            if (instr[k + 1] == 'w'){
                return 'i';
            }
        }
        if(instr[k] == 'b') {
            if (instr[k + 1] == 'n')
                if (instr[k + 2] == 'e'){
                    return 'i';
                }
        }
        /*************Checking for J************/
        if(instr[k] == 'j') {
            return 'j';
        }
        /*************Checking for Pseudo*********/
        if(instr[k] == 'l')
            if(instr[k + 1] == 'a'){
                return'i';
            }
        /************Checking for Directive*******/
        if(instr[k] == '.') {
            if (instr[k + 1] == 's')
                if (instr[k + 2] == 'p')
                    if (instr[k + 3] == 'a')
                        if (instr[k + 4] == 'c')
                            if (instr[k + 5] == 'e')
                                if(instr[k + 6] == '\t'){
                                    return 'd';
                                }
        }
        if(instr[k] == '.') {
            if (instr[k + 1] == 'w')
                if (instr[k + 2] == 'o')
                    if (instr[k + 3] == 'r')
                        if (instr[k + 4] == 'd')
                            if(instr[k + 5] == '\t'){
                                return 'd';
                            }
        }

    }
    return '?';
};

int main() {
    instructionList list[1000];
    char buffer[256];
    int sizeBuffer = sizeof(buffer) / sizeof(buffer[0]);
    int listLocation = 0;
    int address = 0;
    int instrIndex = 0;
    Labels labelList[256];
    char dirOp[32];
    int sizeLabelList = sizeof(labelList[0].name) / sizeof(labelList[0].name[0]);
    int p = 0;
    for (; p < sizeLabelList; p++) { //
        if (buffer[p] == '\000') {
            break;
        }
        buffer[p] = '\000';
    }

//    FILE * fp;
//    fp = fopen("test1.asm","r");

    while (!feof(stdin)) {
        int directive = 0;
        fgets(buffer, 256, stdin);
        int i = 0;
        int set = 0;
        for(; i < sizeBuffer; i++)
        {
            if(buffer[i] == '.') {
                if (buffer[i + 1] == 'd')
                    if (buffer[i + 2] == 'a')
                        if (buffer[i + 3] == 't')
                            if (buffer[i + 4] == 'a') {
                                directive = 1;
                                break;
                            }
                if (buffer[i + 1] == 't')
                    if (buffer[i + 2] == 'e')
                        if (buffer[i + 3] == 'x')
                            if (buffer[i + 4] == 't') {
                                directive = 1;
                                break;
                            }
                if (buffer[i + 1] == 's')
                    if (buffer[i + 2] == 'p')
                        if (buffer[i + 3] == 'a')
                            if (buffer[i + 4] == 'c')
                                if (buffer[i + 5] == 'e') {
                                    i = i + 7;
                                    int count= 0;
                                    while(isdigit(buffer[i]) != 0){
                                        dirOp[count] = buffer[i];
                                        i++;
                                        count++;
                                    }
                                    set = 1;
                                    address = address*4;
                                    list[listLocation].address = address;
                                    address = address + atoi(dirOp);
                                    int sizedirOp= sizeof(dirOp) / sizeof(dirOp[0]);
                                    int p = 0;
                                    for (; p < sizedirOp; p++) { //
                                        if (dirOp[p] == '\000') {
                                            break;
                                        }
                                        dirOp[p] = '\000';
                                    }
                                    listLocation++;
                                    directive = 1;
                                    break;
                                }
                if (buffer[i + 1] == 'w')
                    if (buffer[i + 2] == 'o')
                        if (buffer[i + 3] == 'r')
                            if (buffer[i + 4] == 'd') {
                                i = i + 6;
                                int count= 0;
                                while(isdigit(buffer[i]) != 0){
                                    dirOp[count] = buffer[i];
                                    i++;
                                    count++;
                                }
                                set = 1;
                                address = address*4;
                                list[listLocation].address = address;
                                address = address + (atoi(dirOp)*4);
                                int sizedirOp= sizeof(dirOp) / sizeof(dirOp[0]);
                                int p = 0;
                                for (; p < sizedirOp; p++) { //
                                    if (dirOp[p] == '\000') {
                                        break;
                                    }
                                    dirOp[p] = '\000';
                                }
                                listLocation++;
                                directive = 1;
                                break;
                            }
            }
            else if(buffer[i] == 'l')
                if(buffer[i + 1] == 'a') {
                    char reg[5];
                    char label[50];
                    int regfound = 0;
                    i = i+3;
                    //parse through instruction and build lui and ori
                    int count = 0;
                    while(regfound ==0) {
                        if (buffer[i] == '$') {
                            reg[count] = buffer[i];
                            count++;
                            i++;
                            if (buffer[i] == 't' || buffer[i] == 's') {
                                reg[count] = buffer[i];
                                count++;
                                i++;
                                reg[count] = buffer[i];
                                regfound = 1;
                            }
                        }
                        i++;
                    }
                    i++;
                    count = 0;
                    while (buffer[i] != '\n') {
                        label[count] = buffer[i];
                        count++;
                        i++;
                    }

                    char lui[50] = "lui\t$1,";
                    count = 0;
                    while(label[count]){
                        lui[count+7] =label[count];
                        count++;
                    }
                    char ori[50] = "ori\t";
                    count = 0;
                    while(reg[count]){
                        ori[count+4] =reg[count];
                        count++;
                    }
                    ori[count+4] = ',';
                    ori[count+5] = '$';
                    ori[count+6] = '1';
                    ori[count+7] = ',';
                    count = count+4;
                    int countL = 0;
                    while(label[countL]){
                        ori[count+4] =label[countL];
                        count++;
                        countL++;
                    }

                    count = 0;
                    while(lui[count] != '\000'){
                        set = 1;
                        list[listLocation].instr[instrIndex + count] = lui[count];
                        count++;
                    }
                    set = 1;
                    list[listLocation].address = address*4;
                    listLocation++;
                    address++;
                    count = 0;
                    instrIndex = 0;
                    while(ori[count] != '\000'){
                        set =1;
                        list[listLocation].instr[instrIndex + count] = ori[count];
                        count++;
                    }
                    instrIndex++;
                    list[listLocation].address = address*4;
                    break;
                }
            if(buffer[i] != '\n'){
                list[listLocation].instr[instrIndex] = tolower(buffer[i]);
                instrIndex++;
            }
            if(buffer[i] == '\000'){
                break;
            }
        }

        if(set == 0)
        {
            list[listLocation].address = address*4;
        }
        set = 0;
        if(directive ==0) {
            listLocation++;
            address++;
        }
        instrIndex = 0;
        directive = 0;
        /*********************clearing buffer of any junk*************************/
        int k = 0;
        for (; k < sizeBuffer; k++) { //
            if (buffer[k] == '\000') {
                break;
            }
            buffer[k] = '\000';
        }
    }
    int sizeList = sizeof(list) / sizeof(list[0]);
    int sizeInstr = sizeof(list[0].instr) / sizeof(list[0].instr[0]);
    int counter = 0;
    int counter1 = 0;
    int i = 0;
    for(; i < sizeList; i++)
    {
        int j = 0;
        for(; j < sizeInstr; j++) {
            if(list[i].instr[j] == ':'){
                j--;
                while(j !=-1)
                {
                    labelList[counter].name[j] = list[i].instr[j];
                    j--;
                }
                labelList[counter].address = list[i].address;
                counter ++;
                break;
            }if(list[i].instr[j] == '\n'){

            }
        }

    }

    int k = 0;
    for(; k < sizeList; k++)
    {
        if(list[k].instr[0] == 0) {
            break;
        }else{
            int len = strlen(list[k].instr);
            char type = checkType(list[k].instr, len);
            if (type == 'r') {
                instruction tempRinst = buildRinst(list[k].instr,len,list[k].address);
                buildBinInst(tempRinst);
            } else if (type == 'i') {
                int sizeLabelList = sizeof(labelList) / sizeof(labelList[0]);
                instruction tempIinst = buildIinst(list[k].instr,len,list[k].address, labelList, sizeLabelList);
                buildBinInst(tempIinst);
            } else if (type == 'j') {
                int sizeLabelList = sizeof(labelList) / sizeof(labelList[0]);
                instruction tempJinst = buildJinst(list[k].instr,len,list[k].address, labelList, sizeLabelList);
                buildBinInst(tempJinst);
            }
        }
    }
    return 0;
}