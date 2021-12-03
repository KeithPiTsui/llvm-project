define i32 @sum(i32 %a, i32 %b, i32 %c, i32 %d, i32 %e, i32 %f, i32 %g, i32 %h) {
%1 = add i32 %a, %b
%2 = add i32 %1, %c
%3 = add i32 %2, %d
%4 = add i32 %3, %e
%5 = add i32 %4, %f
%6 = add i32 %5, %g
%7 = add i32 %6, %h
ret i32 %7
}


define i32 @lvn(i32 %a, i32 %b) {
%1 = add i32 %a, %b
%2 = add i32 %a, %b
%3 = add i32 %1, %2
ret i32 %3
}
