#include<stdio.h>
#include<math.h>
#include<string.h>
#define max 1000

int multi(int * fact, int multiplier, int len) {
    int carry = 0;
    for(int k = 0; k < len; k++) {
        int mul = fact[k] * multiplier + carry;
        fact[k] = mul % 10;
        carry = mul / 10;
    }
    while(carry) {
        fact[len++] = carry % 10;
        carry /= 10;
    }
    return len;
}
int add(int * fact, int * addend, int len) {
    int carry = 0;
    for(int k = 0; k < len; k++) {
        int sum = fact[k] + addend[k] + carry;
        addend[k] = sum % 10;
        carry = sum / 10;
    }
    while(carry) {
        addend[len++] = carry % 10;
        carry /= 10;
    }
    return len;
}
int main() {
    int n; 
    scanf("%d", &n);   // 求n个阶乘的和//
    int fact[max];
    int addend[max];
    memset(fact, 0, sizeof(fact));
    memset(addend, 0, sizeof(addend));
    fact[0] = 1;
    int fact_len = 1;
    int addend_len = 0;                               
    for(int i = 1; i <= n; i++) {
        fact_len = multi(fact, i, fact_len);          
        addend_len = add(fact, addend, fact_len);
    }
    for(int i = addend_len - 1; i >= 0; i--) {
        printf("%d", addend[i]);
    }
    return 0;
}