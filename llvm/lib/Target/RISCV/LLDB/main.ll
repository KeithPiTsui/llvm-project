; ModuleID = 'main.c'
source_filename = "main.c"
target datalayout = "e-m:e-p:64:64-i64:64-i128:128-n64-S128"
target triple = "riscv64-unknown-unknown"

@.str = private unnamed_addr constant [14 x i8] c"hello world!\0A\00", align 1

; Function Attrs: noinline nounwind optnone
define dso_local signext i32 @add(i32 signext %a, i32 signext %b, i32 signext %c, i32 signext %d, i32 signext %e, i32 signext %f, i32 signext %g, i32 signext %h, i32 %i, i32 %j) #0 {
entry:
  %a.addr = alloca i32, align 4
  %b.addr = alloca i32, align 4
  %c.addr = alloca i32, align 4
  %d.addr = alloca i32, align 4
  %e.addr = alloca i32, align 4
  %f.addr = alloca i32, align 4
  %g.addr = alloca i32, align 4
  %h.addr = alloca i32, align 4
  %i.addr = alloca i32, align 4
  %j.addr = alloca i32, align 4
  store i32 %a, i32* %a.addr, align 4
  store i32 %b, i32* %b.addr, align 4
  store i32 %c, i32* %c.addr, align 4
  store i32 %d, i32* %d.addr, align 4
  store i32 %e, i32* %e.addr, align 4
  store i32 %f, i32* %f.addr, align 4
  store i32 %g, i32* %g.addr, align 4
  store i32 %h, i32* %h.addr, align 4
  store i32 %i, i32* %i.addr, align 4
  store i32 %j, i32* %j.addr, align 4
  %0 = load i32, i32* %a.addr, align 4
  %1 = load i32, i32* %b.addr, align 4
  %add = add nsw i32 %0, %1
  %2 = load i32, i32* %c.addr, align 4
  %add1 = add nsw i32 %add, %2
  %3 = load i32, i32* %d.addr, align 4
  %add2 = add nsw i32 %add1, %3
  %4 = load i32, i32* %e.addr, align 4
  %add3 = add nsw i32 %add2, %4
  %5 = load i32, i32* %f.addr, align 4
  %add4 = add nsw i32 %add3, %5
  %6 = load i32, i32* %g.addr, align 4
  %add5 = add nsw i32 %add4, %6
  %7 = load i32, i32* %h.addr, align 4
  %add6 = add nsw i32 %add5, %7
  %8 = load i32, i32* %i.addr, align 4
  %add7 = add nsw i32 %add6, %8
  %9 = load i32, i32* %j.addr, align 4
  %add8 = add nsw i32 %add7, %9
  ret i32 %add8
}

; Function Attrs: noinline nounwind optnone
define dso_local signext i32 @main(i32 signext %argc, i8** %argv) #0 {
entry:
  %retval = alloca i32, align 4
  %argc.addr = alloca i32, align 4
  %argv.addr = alloca i8**, align 8
  store i32 0, i32* %retval, align 4
  store i32 %argc, i32* %argc.addr, align 4
  store i8** %argv, i8*** %argv.addr, align 8
  %call = call signext i32 (i8*, ...) @printf(i8* getelementptr inbounds ([14 x i8], [14 x i8]* @.str, i64 0, i64 0))
  %call1 = call signext i32 @add(i32 signext 1, i32 signext 2, i32 signext 3, i32 signext 4, i32 signext 5, i32 signext 6, i32 signext 7, i32 signext 8, i32 9, i32 10)
  ret i32 %call1
}

declare dso_local signext i32 @printf(i8*, ...) #1

attributes #0 = { noinline nounwind optnone "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "frame-pointer"="all" "less-precise-fpmad"="false" "min-legal-vector-width"="0" "no-infs-fp-math"="false" "no-jump-tables"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-features"="+a,+c,+d,+f,+m,+relax,-save-restore" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #1 = { "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "frame-pointer"="all" "less-precise-fpmad"="false" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-features"="+a,+c,+d,+f,+m,+relax,-save-restore" "unsafe-fp-math"="false" "use-soft-float"="false" }

!llvm.module.flags = !{!0, !1, !2}
!llvm.ident = !{!3}

!0 = !{i32 1, !"wchar_size", i32 4}
!1 = !{i32 1, !"target-abi", !"lp64d"}
!2 = !{i32 1, !"SmallDataLimit", i32 8}
!3 = !{!"clang version 11.1.0 (git@github.com:KeithPiTsui/llvm-project.git e8ab6a7603b475cd2821eb2727e2e9c72f311423)"}
