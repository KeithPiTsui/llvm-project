define i32 @main() {
%1 = alloca i32
store i32 1, i32* %1
%2 = load i32, i32* %1
%3 = add i32 %2, 2
ret i32 %3
}
