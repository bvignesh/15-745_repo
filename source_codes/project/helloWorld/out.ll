; ModuleID = 'out.bc'
target datalayout = "e-m:e-p:32:32-f64:32:64-f80:32-n8:16:32-S128"
target triple = "i386-pc-linux-gnu"

@threadCount = global i32 0, align 4
@.str = private unnamed_addr constant [11 x i8] c"Beep [%d]\0A\00", align 1
@arg1 = common global i32 0, align 4

; Function Attrs: nounwind
define i32 @main(i32 %argc, i8** %argv) #0 {
entry:
  %retval = alloca i32, align 4
  %argc.addr = alloca i32, align 4
  %argv.addr = alloca i8**, align 4
  %i = alloca i32, align 4
  %argIndex = getelementptr inbounds i8** %argv, i32 1
  %arg2 = load i8** %argIndex, align 4
  %atoiResult = call i32 @atoi(i8* %arg2)
  store i32 %atoiResult, i32* @threadCount, align 4
  store i32 0, i32* %retval
  store i32 %argc, i32* %argc.addr, align 4
  store i8** %argv, i8*** %argv.addr, align 4
  store i32 128, i32* %i, align 4
  %0 = load i32* %argc.addr, align 4
  store i32 %0, i32* %i, align 4
  %1 = load i32* %i, align 4
  %currentVal = load i32* @threadCount, align 4
  %call = call i32 (i8*, ...)* @printf(i8* getelementptr inbounds ([11 x i8]* @.str, i32 0, i32 0), i32 %currentVal)
  ret i32 250
}

declare i32 @printf(i8*, ...) #1

declare i32 @atoi(i8*)

attributes #0 = { nounwind "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "stack-protector-buffer-size"="8" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #1 = { "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "stack-protector-buffer-size"="8" "unsafe-fp-math"="false" "use-soft-float"="false" }

!llvm.ident = !{!0}

!0 = metadata !{metadata !"clang version 3.5.0 (branches/release_35 225468)"}
