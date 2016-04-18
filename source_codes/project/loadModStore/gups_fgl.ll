; ModuleID = 'gups_fgl.bc'
target datalayout = "e-m:e-p:32:32-f64:32:64-f80:32-n8:16:32-S128"
target triple = "i386-pc-linux-gnu"

%"class.std::ios_base::Init" = type { i8 }
%union.pthread_mutex_t = type { %"struct.(anonymous union)::__pthread_mutex_s" }
%"struct.(anonymous union)::__pthread_mutex_s" = type { i32, i32, i32, i32, i32, %union.anon }
%union.anon = type { %struct.__pthread_internal_slist }
%struct.__pthread_internal_slist = type { %struct.__pthread_internal_slist* }
%struct.entry = type { i32, %union.pthread_mutex_t }
%struct._IO_FILE = type { i32, i8*, i8*, i8*, i8*, i8*, i8*, i8*, i8*, i8*, i8*, i8*, %struct._IO_marker*, %struct._IO_FILE*, i32, i32, i32, i16, i8, [1 x i8], i8*, i64, i8*, i8*, i8*, i8*, i32, i32, [40 x i8] }
%struct._IO_marker = type { %struct._IO_marker*, %struct._IO_FILE*, i32 }
%"class.std::basic_ostream" = type { i32 (...)**, %"class.std::basic_ios" }
%"class.std::basic_ios" = type { %"class.std::ios_base", %"class.std::basic_ostream"*, i8, i8, %"class.std::basic_streambuf"*, %"class.std::ctype"*, %"class.std::num_put"*, %"class.std::num_get"* }
%"class.std::ios_base" = type { i32 (...)**, i32, i32, i32, i32, i32, %"struct.std::ios_base::_Callback_list"*, %"struct.std::ios_base::_Words", [8 x %"struct.std::ios_base::_Words"], i32, %"struct.std::ios_base::_Words"*, %"class.std::locale" }
%"struct.std::ios_base::_Callback_list" = type { %"struct.std::ios_base::_Callback_list"*, void (i32, %"class.std::ios_base"*, i32)*, i32, i32 }
%"struct.std::ios_base::_Words" = type { i8*, i32 }
%"class.std::locale" = type { %"class.std::locale::_Impl"* }
%"class.std::locale::_Impl" = type { i32, %"class.std::locale::facet"**, i32, %"class.std::locale::facet"**, i8** }
%"class.std::locale::facet" = type { i32 (...)**, i32 }
%"class.std::basic_streambuf" = type { i32 (...)**, i8*, i8*, i8*, i8*, i8*, i8*, %"class.std::locale" }
%"class.std::ctype" = type { %"class.std::locale::facet", %struct.__locale_struct*, i8, i32*, i32*, i16*, i8, [256 x i8], [256 x i8], i8 }
%struct.__locale_struct = type { [13 x %struct.__locale_data*], i16*, i32*, i32*, [13 x i8*] }
%struct.__locale_data = type opaque
%"class.std::num_put" = type { %"class.std::locale::facet" }
%"class.std::num_get" = type { %"class.std::locale::facet" }
%union.pthread_mutexattr_t = type { i32 }
%union.pthread_attr_t = type { i32, [32 x i8] }

@_ZStL8__ioinit = internal global %"class.std::ios_base::Init" zeroinitializer, align 1
@__dso_handle = external global i8
@INPUT_SIZE = global i32 0, align 4
@TABLE_SIZE = global i32 0, align 4
@data = global i32* null, align 4
@mutex_all = global %union.pthread_mutex_t zeroinitializer, align 4
@hash_table = global %struct.entry* null, align 4
@tids = global [8 x i32] zeroinitializer, align 4
@.str = private unnamed_addr constant [41 x i8] c"usage: ./gups <input size> <table size>\0A\00", align 1
@stderr = external global %struct._IO_FILE*
@.str1 = private unnamed_addr constant [45 x i8] c"Error - pthread_create() returned code - %d\0A\00", align 1
@_ZSt4cout = external global %"class.std::basic_ostream"
@.str2 = private unnamed_addr constant [12 x i8] c"ERROR_GEN: \00", align 1
@.str3 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@.str4 = private unnamed_addr constant [8 x i8] c"ERROR: \00", align 1
@.str5 = private unnamed_addr constant [2 x i8] c" \00", align 1
@llvm.global_ctors = appending global [1 x { i32, void ()*, i8* }] [{ i32, void ()*, i8* } { i32 65535, void ()* @_GLOBAL__sub_I_gups_fgl.cpp, i8* null }]

define internal void @__cxx_global_var_init() section ".text.startup" {
entry:
  call void @_ZNSt8ios_base4InitC1Ev(%"class.std::ios_base::Init"* @_ZStL8__ioinit)
  %0 = call i32 @__cxa_atexit(void (i8*)* bitcast (void (%"class.std::ios_base::Init"*)* @_ZNSt8ios_base4InitD1Ev to void (i8*)*), i8* getelementptr inbounds (%"class.std::ios_base::Init"* @_ZStL8__ioinit, i32 0, i32 0), i8* @__dso_handle) #1
  ret void
}

declare void @_ZNSt8ios_base4InitC1Ev(%"class.std::ios_base::Init"*) #0

declare void @_ZNSt8ios_base4InitD1Ev(%"class.std::ios_base::Init"*) #0

; Function Attrs: nounwind
declare i32 @__cxa_atexit(void (i8*)*, i8*, i8*) #1

; Function Attrs: nounwind
define void @_Z12start_regioni(i32 %i) #2 {
entry:
  %i.addr = alloca i32, align 4
  store i32 %i, i32* %i.addr, align 4
  %call = call i32 @pthread_mutex_lock(%union.pthread_mutex_t* @mutex_all) #1
  ret void
}

; Function Attrs: nounwind
declare i32 @pthread_mutex_lock(%union.pthread_mutex_t*) #2

; Function Attrs: nounwind
define void @_Z10end_regioni(i32 %i) #2 {
entry:
  %i.addr = alloca i32, align 4
  store i32 %i, i32* %i.addr, align 4
  %call = call i32 @pthread_mutex_unlock(%union.pthread_mutex_t* @mutex_all) #1
  ret void
}

; Function Attrs: nounwind
declare i32 @pthread_mutex_unlock(%union.pthread_mutex_t*) #2

; Function Attrs: nounwind
define void @_Z8do_mergev() #2 {
entry:
  ret void
}

; Function Attrs: nounwind
define void @_Z8do_splitv() #2 {
entry:
  ret void
}

; Function Attrs: nounwind
define void @_Z13register_tidsPiii(i32* %tids, i32 %a, i32 %b) #2 {
entry:
  %tids.addr = alloca i32*, align 4
  %a.addr = alloca i32, align 4
  %b.addr = alloca i32, align 4
  store i32* %tids, i32** %tids.addr, align 4
  store i32 %a, i32* %a.addr, align 4
  store i32 %b, i32* %b.addr, align 4
  ret void
}

define i32 @main(i32 %argc, i8** %argv) #0 {
entry:
  %retval = alloca i32, align 4
  %argc.addr = alloca i32, align 4
  %argv.addr = alloca i8**, align 4
  %threads = alloca [8 x i32], align 4
  %ret = alloca i32, align 4
  %i = alloca i32, align 4
  %i10 = alloca i32, align 4
  %i24 = alloca i32, align 4
  store i32 0, i32* %retval
  store i32 %argc, i32* %argc.addr, align 4
  store i8** %argv, i8*** %argv.addr, align 4
  %0 = load i32* %argc.addr, align 4
  %cmp = icmp ne i32 %0, 3
  br i1 %cmp, label %if.then, label %if.end

if.then:                                          ; preds = %entry
  %call = call i32 (i8*, ...)* @printf(i8* getelementptr inbounds ([41 x i8]* @.str, i32 0, i32 0))
  store i32 -1, i32* %retval
  br label %return

if.end:                                           ; preds = %entry
  %call1 = call i32 @pthread_mutex_init(%union.pthread_mutex_t* @mutex_all, %union.pthread_mutexattr_t* null) #1
  %1 = load i8*** %argv.addr, align 4
  %arrayidx = getelementptr inbounds i8** %1, i32 1
  %2 = load i8** %arrayidx, align 4
  %call2 = call i32 @atoi(i8* %2) #6
  store i32 %call2, i32* @INPUT_SIZE, align 4
  %3 = load i8*** %argv.addr, align 4
  %arrayidx3 = getelementptr inbounds i8** %3, i32 2
  %4 = load i8** %arrayidx3, align 4
  %call4 = call i32 @atoi(i8* %4) #6
  store i32 %call4, i32* @TABLE_SIZE, align 4
  call void @_Z13register_tidsPiii(i32* getelementptr inbounds ([8 x i32]* @tids, i32 0, i32 0), i32 0, i32 0)
  call void @_Z7read_inv()
  %5 = load i32* @TABLE_SIZE, align 4
  %mul = mul i32 28, %5
  %call5 = call noalias i8* @malloc(i32 %mul) #1
  %6 = bitcast i8* %call5 to %struct.entry*
  store %struct.entry* %6, %struct.entry** @hash_table, align 4
  store i32 0, i32* %i, align 4
  br label %for.cond

for.cond:                                         ; preds = %for.inc, %if.end
  %7 = load i32* %i, align 4
  %8 = load i32* @TABLE_SIZE, align 4
  %cmp6 = icmp slt i32 %7, %8
  br i1 %cmp6, label %for.body, label %for.end

for.body:                                         ; preds = %for.cond
  %9 = load i32* %i, align 4
  %10 = load %struct.entry** @hash_table, align 4
  %arrayidx7 = getelementptr inbounds %struct.entry* %10, i32 %9
  %data = getelementptr inbounds %struct.entry* %arrayidx7, i32 0, i32 0
  store i32 0, i32* %data, align 4
  %11 = load i32* %i, align 4
  %12 = load %struct.entry** @hash_table, align 4
  %arrayidx8 = getelementptr inbounds %struct.entry* %12, i32 %11
  %mutex = getelementptr inbounds %struct.entry* %arrayidx8, i32 0, i32 1
  %call9 = call i32 @pthread_mutex_init(%union.pthread_mutex_t* %mutex, %union.pthread_mutexattr_t* null) #1
  br label %for.inc

for.inc:                                          ; preds = %for.body
  %13 = load i32* %i, align 4
  %inc = add nsw i32 %13, 1
  store i32 %inc, i32* %i, align 4
  br label %for.cond

for.end:                                          ; preds = %for.cond
  store i32 0, i32* %i10, align 4
  br label %for.cond11

for.cond11:                                       ; preds = %for.inc21, %for.end
  %14 = load i32* %i10, align 4
  %cmp12 = icmp slt i32 %14, 8
  br i1 %cmp12, label %for.body13, label %for.end23

for.body13:                                       ; preds = %for.cond11
  %15 = load i32* %i10, align 4
  %16 = load i32* %i10, align 4
  %arrayidx14 = getelementptr inbounds [8 x i32]* @tids, i32 0, i32 %16
  store i32 %15, i32* %arrayidx14, align 4
  %17 = load i32* %i10, align 4
  %arrayidx15 = getelementptr inbounds [8 x i32]* %threads, i32 0, i32 %17
  %18 = load i32* %i10, align 4
  %arrayidx16 = getelementptr inbounds [8 x i32]* @tids, i32 0, i32 %18
  %19 = bitcast i32* %arrayidx16 to i8*
  %call17 = call i32 @pthread_create(i32* %arrayidx15, %union.pthread_attr_t* null, i8* (i8*)* @_Z13incr_functionPv, i8* %19) #1
  store i32 %call17, i32* %ret, align 4
  %20 = load i32* %ret, align 4
  %tobool = icmp ne i32 %20, 0
  br i1 %tobool, label %if.then18, label %if.end20

if.then18:                                        ; preds = %for.body13
  %21 = load %struct._IO_FILE** @stderr, align 4
  %22 = load i32* %ret, align 4
  %call19 = call i32 (%struct._IO_FILE*, i8*, ...)* @fprintf(%struct._IO_FILE* %21, i8* getelementptr inbounds ([45 x i8]* @.str1, i32 0, i32 0), i32 %22)
  call void @exit(i32 1) #7
  unreachable

if.end20:                                         ; preds = %for.body13
  br label %for.inc21

for.inc21:                                        ; preds = %if.end20
  %23 = load i32* %i10, align 4
  %inc22 = add nsw i32 %23, 1
  store i32 %inc22, i32* %i10, align 4
  br label %for.cond11

for.end23:                                        ; preds = %for.cond11
  store i32 0, i32* %i24, align 4
  br label %for.cond25

for.cond25:                                       ; preds = %for.inc30, %for.end23
  %24 = load i32* %i24, align 4
  %cmp26 = icmp slt i32 %24, 8
  br i1 %cmp26, label %for.body27, label %for.end32

for.body27:                                       ; preds = %for.cond25
  %25 = load i32* %i24, align 4
  %arrayidx28 = getelementptr inbounds [8 x i32]* %threads, i32 0, i32 %25
  %26 = load i32* %arrayidx28, align 4
  %call29 = call i32 @pthread_join(i32 %26, i8** null)
  br label %for.inc30

for.inc30:                                        ; preds = %for.body27
  %27 = load i32* %i24, align 4
  %inc31 = add nsw i32 %27, 1
  store i32 %inc31, i32* %i24, align 4
  br label %for.cond25

for.end32:                                        ; preds = %for.cond25
  call void @_Z9write_outv()
  store i32 0, i32* %retval
  br label %return

return:                                           ; preds = %for.end32, %if.then
  %28 = load i32* %retval
  ret i32 %28
}

declare i32 @printf(i8*, ...) #0

; Function Attrs: nounwind
declare i32 @pthread_mutex_init(%union.pthread_mutex_t*, %union.pthread_mutexattr_t*) #2

; Function Attrs: nounwind readonly
declare i32 @atoi(i8*) #3

; Function Attrs: nounwind
define void @_Z7read_inv() #2 {
entry:
  %i = alloca i32, align 4
  %0 = load i32* @INPUT_SIZE, align 4
  %mul = mul i32 %0, 4
  %call = call noalias i8* @malloc(i32 %mul) #1
  %1 = bitcast i8* %call to i32*
  store i32* %1, i32** @data, align 4
  %call1 = call i32 @time(i32* null) #1
  call void @srand(i32 %call1) #1
  store i32 0, i32* %i, align 4
  br label %for.cond

for.cond:                                         ; preds = %for.inc, %entry
  %2 = load i32* %i, align 4
  %3 = load i32* @INPUT_SIZE, align 4
  %cmp = icmp slt i32 %2, %3
  br i1 %cmp, label %for.body, label %for.end

for.body:                                         ; preds = %for.cond
  %call2 = call i32 @rand() #1
  %4 = load i32* @TABLE_SIZE, align 4
  %rem = srem i32 %call2, %4
  %5 = load i32* %i, align 4
  %6 = load i32** @data, align 4
  %arrayidx = getelementptr inbounds i32* %6, i32 %5
  store i32 %rem, i32* %arrayidx, align 4
  br label %for.inc

for.inc:                                          ; preds = %for.body
  %7 = load i32* %i, align 4
  %inc = add nsw i32 %7, 1
  store i32 %inc, i32* %i, align 4
  br label %for.cond

for.end:                                          ; preds = %for.cond
  ret void
}

; Function Attrs: nounwind
declare noalias i8* @malloc(i32) #2

; Function Attrs: nounwind
declare i32 @pthread_create(i32*, %union.pthread_attr_t*, i8* (i8*)*, i8*) #2

define i8* @_Z13incr_functionPv(i8* %dummy) #0 {
entry:
  %dummy.addr = alloca i8*, align 4
  %tid = alloca i32, align 4
  %start = alloca i32, align 4
  %end = alloca i32, align 4
  %ref.tmp = alloca i32, align 4
  %i = alloca i32, align 4
  %loc = alloca i32, align 4
  store i8* %dummy, i8** %dummy.addr, align 4
  %0 = load i8** %dummy.addr, align 4
  %1 = bitcast i8* %0 to i32*
  %2 = load i32* %1, align 4
  store i32 %2, i32* %tid, align 4
  %3 = load i32* @INPUT_SIZE, align 4
  %div = sdiv i32 %3, 8
  %4 = load i32* %tid, align 4
  %mul = mul nsw i32 %div, %4
  store i32 %mul, i32* %start, align 4
  %5 = load i32* @INPUT_SIZE, align 4
  %div1 = sdiv i32 %5, 8
  %6 = load i32* %tid, align 4
  %add = add nsw i32 %6, 1
  %mul2 = mul nsw i32 %div1, %add
  store i32 %mul2, i32* %ref.tmp
  %call = call dereferenceable(4) i32* @_ZSt3minIiERKT_S2_S2_(i32* dereferenceable(4) %ref.tmp, i32* dereferenceable(4) @INPUT_SIZE)
  %7 = load i32* %call
  store i32 %7, i32* %end, align 4
  %8 = load i32* %start, align 4
  store i32 %8, i32* %i, align 4
  br label %for.cond

for.cond:                                         ; preds = %for.inc, %entry
  %9 = load i32* %i, align 4
  %10 = load i32* %end, align 4
  %cmp = icmp slt i32 %9, %10
  br i1 %cmp, label %for.body, label %for.end

for.body:                                         ; preds = %for.cond
  %11 = load i32* %i, align 4
  %12 = load i32** @data, align 4
  %arrayidx = getelementptr inbounds i32* %12, i32 %11
  %13 = load i32* %arrayidx, align 4
  store i32 %13, i32* %loc, align 4
  %14 = load i32* %loc, align 4
  %15 = load %struct.entry** @hash_table, align 4
  %arrayidx3 = getelementptr inbounds %struct.entry* %15, i32 %14
  %mutex = getelementptr inbounds %struct.entry* %arrayidx3, i32 0, i32 1
  %call4 = call i32 @pthread_mutex_lock(%union.pthread_mutex_t* %mutex) #1
  %16 = load i32* %loc, align 4
  %17 = load %struct.entry** @hash_table, align 4
  %arrayidx5 = getelementptr inbounds %struct.entry* %17, i32 %16
  %data = getelementptr inbounds %struct.entry* %arrayidx5, i32 0, i32 0
  %18 = load i32* %data, align 4
  %inc = add nsw i32 %18, 1
  store i32 %inc, i32* %data, align 4
  %19 = load i32* %loc, align 4
  %20 = load %struct.entry** @hash_table, align 4
  %arrayidx6 = getelementptr inbounds %struct.entry* %20, i32 %19
  %mutex7 = getelementptr inbounds %struct.entry* %arrayidx6, i32 0, i32 1
  %call8 = call i32 @pthread_mutex_unlock(%union.pthread_mutex_t* %mutex7) #1
  br label %for.inc

for.inc:                                          ; preds = %for.body
  %21 = load i32* %i, align 4
  %inc9 = add nsw i32 %21, 1
  store i32 %inc9, i32* %i, align 4
  br label %for.cond

for.end:                                          ; preds = %for.cond
  ret i8* null
}

declare i32 @fprintf(%struct._IO_FILE*, i8*, ...) #0

; Function Attrs: noreturn nounwind
declare void @exit(i32) #4

declare i32 @pthread_join(i32, i8**) #0

define void @_Z9write_outv() #0 {
entry:
  %error = alloca i32, align 4
  %err_0 = alloca i32, align 4
  %i = alloca i32, align 4
  %i3 = alloca i32, align 4
  %err_g = alloca i32, align 4
  %err_l = alloca i32, align 4
  %i28 = alloca i32, align 4
  store i32 0, i32* %error, align 4
  store i32 0, i32* %err_0, align 4
  store i32 0, i32* %i, align 4
  br label %for.cond

for.cond:                                         ; preds = %for.inc, %entry
  %0 = load i32* %i, align 4
  %1 = load i32* @TABLE_SIZE, align 4
  %cmp = icmp slt i32 %0, %1
  br i1 %cmp, label %for.body, label %for.end

for.body:                                         ; preds = %for.cond
  %2 = load i32* %i, align 4
  %3 = load %struct.entry** @hash_table, align 4
  %arrayidx = getelementptr inbounds %struct.entry* %3, i32 %2
  %data = getelementptr inbounds %struct.entry* %arrayidx, i32 0, i32 0
  %4 = load i32* %data, align 4
  %cmp1 = icmp eq i32 %4, 0
  br i1 %cmp1, label %if.then, label %if.end

if.then:                                          ; preds = %for.body
  %5 = load i32* %err_0, align 4
  %inc = add nsw i32 %5, 1
  store i32 %inc, i32* %err_0, align 4
  br label %if.end

if.end:                                           ; preds = %if.then, %for.body
  br label %for.inc

for.inc:                                          ; preds = %if.end
  %6 = load i32* %i, align 4
  %inc2 = add nsw i32 %6, 1
  store i32 %inc2, i32* %i, align 4
  br label %for.cond

for.end:                                          ; preds = %for.cond
  store i32 0, i32* %i3, align 4
  br label %for.cond4

for.cond4:                                        ; preds = %for.inc20, %for.end
  %7 = load i32* %i3, align 4
  %8 = load i32* @INPUT_SIZE, align 4
  %cmp5 = icmp slt i32 %7, %8
  br i1 %cmp5, label %for.body6, label %for.end22

for.body6:                                        ; preds = %for.cond4
  %9 = load i32* %i3, align 4
  %10 = load i32** @data, align 4
  %arrayidx7 = getelementptr inbounds i32* %10, i32 %9
  %11 = load i32* %arrayidx7, align 4
  %12 = load %struct.entry** @hash_table, align 4
  %arrayidx8 = getelementptr inbounds %struct.entry* %12, i32 %11
  %data9 = getelementptr inbounds %struct.entry* %arrayidx8, i32 0, i32 0
  %13 = load i32* %data9, align 4
  %dec = add nsw i32 %13, -1
  store i32 %dec, i32* %data9, align 4
  %14 = load i32* %i3, align 4
  %15 = load i32** @data, align 4
  %arrayidx10 = getelementptr inbounds i32* %15, i32 %14
  %16 = load i32* %arrayidx10, align 4
  %17 = load i32* @TABLE_SIZE, align 4
  %cmp11 = icmp sgt i32 %16, %17
  br i1 %cmp11, label %if.then12, label %if.end14

if.then12:                                        ; preds = %for.body6
  %18 = load i32* %error, align 4
  %inc13 = add nsw i32 %18, 1
  store i32 %inc13, i32* %error, align 4
  br label %if.end14

if.end14:                                         ; preds = %if.then12, %for.body6
  %19 = load i32* %i3, align 4
  %20 = load i32** @data, align 4
  %arrayidx15 = getelementptr inbounds i32* %20, i32 %19
  %21 = load i32* %arrayidx15, align 4
  %cmp16 = icmp slt i32 %21, 0
  br i1 %cmp16, label %if.then17, label %if.end19

if.then17:                                        ; preds = %if.end14
  %22 = load i32* %error, align 4
  %inc18 = add nsw i32 %22, 1
  store i32 %inc18, i32* %error, align 4
  br label %if.end19

if.end19:                                         ; preds = %if.then17, %if.end14
  br label %for.inc20

for.inc20:                                        ; preds = %if.end19
  %23 = load i32* %i3, align 4
  %inc21 = add nsw i32 %23, 1
  store i32 %inc21, i32* %i3, align 4
  br label %for.cond4

for.end22:                                        ; preds = %for.cond4
  %24 = load i32* %error, align 4
  %cmp23 = icmp ne i32 %24, 0
  br i1 %cmp23, label %if.then24, label %if.end27

if.then24:                                        ; preds = %for.end22
  %call = call dereferenceable(140) %"class.std::basic_ostream"* @_ZStlsISt11char_traitsIcEERSt13basic_ostreamIcT_ES5_PKc(%"class.std::basic_ostream"* dereferenceable(140) @_ZSt4cout, i8* getelementptr inbounds ([12 x i8]* @.str2, i32 0, i32 0))
  %25 = load i32* %error, align 4
  %call25 = call dereferenceable(140) %"class.std::basic_ostream"* @_ZNSolsEi(%"class.std::basic_ostream"* %call, i32 %25)
  %call26 = call dereferenceable(140) %"class.std::basic_ostream"* @_ZStlsISt11char_traitsIcEERSt13basic_ostreamIcT_ES5_PKc(%"class.std::basic_ostream"* dereferenceable(140) %call25, i8* getelementptr inbounds ([2 x i8]* @.str3, i32 0, i32 0))
  br label %if.end27

if.end27:                                         ; preds = %if.then24, %for.end22
  store i32 0, i32* %error, align 4
  store i32 0, i32* %err_g, align 4
  store i32 0, i32* %err_l, align 4
  store i32 0, i32* %i28, align 4
  br label %for.cond29

for.cond29:                                       ; preds = %for.inc50, %if.end27
  %26 = load i32* %i28, align 4
  %27 = load i32* @TABLE_SIZE, align 4
  %cmp30 = icmp slt i32 %26, %27
  br i1 %cmp30, label %for.body31, label %for.end52

for.body31:                                       ; preds = %for.cond29
  %28 = load i32* %i28, align 4
  %29 = load %struct.entry** @hash_table, align 4
  %arrayidx32 = getelementptr inbounds %struct.entry* %29, i32 %28
  %data33 = getelementptr inbounds %struct.entry* %arrayidx32, i32 0, i32 0
  %30 = load i32* %data33, align 4
  %cmp34 = icmp ne i32 %30, 0
  br i1 %cmp34, label %if.then35, label %if.end37

if.then35:                                        ; preds = %for.body31
  %31 = load i32* %error, align 4
  %inc36 = add nsw i32 %31, 1
  store i32 %inc36, i32* %error, align 4
  br label %if.end37

if.end37:                                         ; preds = %if.then35, %for.body31
  %32 = load i32* %i28, align 4
  %33 = load %struct.entry** @hash_table, align 4
  %arrayidx38 = getelementptr inbounds %struct.entry* %33, i32 %32
  %data39 = getelementptr inbounds %struct.entry* %arrayidx38, i32 0, i32 0
  %34 = load i32* %data39, align 4
  %cmp40 = icmp slt i32 %34, 0
  br i1 %cmp40, label %if.then41, label %if.end43

if.then41:                                        ; preds = %if.end37
  %35 = load i32* %err_l, align 4
  %inc42 = add nsw i32 %35, 1
  store i32 %inc42, i32* %err_l, align 4
  br label %if.end43

if.end43:                                         ; preds = %if.then41, %if.end37
  %36 = load i32* %i28, align 4
  %37 = load %struct.entry** @hash_table, align 4
  %arrayidx44 = getelementptr inbounds %struct.entry* %37, i32 %36
  %data45 = getelementptr inbounds %struct.entry* %arrayidx44, i32 0, i32 0
  %38 = load i32* %data45, align 4
  %cmp46 = icmp sgt i32 %38, 0
  br i1 %cmp46, label %if.then47, label %if.end49

if.then47:                                        ; preds = %if.end43
  %39 = load i32* %err_g, align 4
  %inc48 = add nsw i32 %39, 1
  store i32 %inc48, i32* %err_g, align 4
  br label %if.end49

if.end49:                                         ; preds = %if.then47, %if.end43
  br label %for.inc50

for.inc50:                                        ; preds = %if.end49
  %40 = load i32* %i28, align 4
  %inc51 = add nsw i32 %40, 1
  store i32 %inc51, i32* %i28, align 4
  br label %for.cond29

for.end52:                                        ; preds = %for.cond29
  %41 = load i32* %error, align 4
  %cmp53 = icmp ne i32 %41, 0
  br i1 %cmp53, label %if.then54, label %if.end64

if.then54:                                        ; preds = %for.end52
  %call55 = call dereferenceable(140) %"class.std::basic_ostream"* @_ZStlsISt11char_traitsIcEERSt13basic_ostreamIcT_ES5_PKc(%"class.std::basic_ostream"* dereferenceable(140) @_ZSt4cout, i8* getelementptr inbounds ([8 x i8]* @.str4, i32 0, i32 0))
  %42 = load i32* %error, align 4
  %call56 = call dereferenceable(140) %"class.std::basic_ostream"* @_ZNSolsEi(%"class.std::basic_ostream"* %call55, i32 %42)
  %call57 = call dereferenceable(140) %"class.std::basic_ostream"* @_ZStlsISt11char_traitsIcEERSt13basic_ostreamIcT_ES5_PKc(%"class.std::basic_ostream"* dereferenceable(140) %call56, i8* getelementptr inbounds ([2 x i8]* @.str5, i32 0, i32 0))
  %43 = load i32* %err_l, align 4
  %call58 = call dereferenceable(140) %"class.std::basic_ostream"* @_ZNSolsEi(%"class.std::basic_ostream"* %call57, i32 %43)
  %call59 = call dereferenceable(140) %"class.std::basic_ostream"* @_ZStlsISt11char_traitsIcEERSt13basic_ostreamIcT_ES5_PKc(%"class.std::basic_ostream"* dereferenceable(140) %call58, i8* getelementptr inbounds ([2 x i8]* @.str5, i32 0, i32 0))
  %44 = load i32* %err_g, align 4
  %call60 = call dereferenceable(140) %"class.std::basic_ostream"* @_ZNSolsEi(%"class.std::basic_ostream"* %call59, i32 %44)
  %call61 = call dereferenceable(140) %"class.std::basic_ostream"* @_ZStlsISt11char_traitsIcEERSt13basic_ostreamIcT_ES5_PKc(%"class.std::basic_ostream"* dereferenceable(140) %call60, i8* getelementptr inbounds ([2 x i8]* @.str5, i32 0, i32 0))
  %45 = load i32* %err_0, align 4
  %call62 = call dereferenceable(140) %"class.std::basic_ostream"* @_ZNSolsEi(%"class.std::basic_ostream"* %call61, i32 %45)
  %call63 = call dereferenceable(140) %"class.std::basic_ostream"* @_ZStlsISt11char_traitsIcEERSt13basic_ostreamIcT_ES5_PKc(%"class.std::basic_ostream"* dereferenceable(140) %call62, i8* getelementptr inbounds ([2 x i8]* @.str3, i32 0, i32 0))
  br label %if.end64

if.end64:                                         ; preds = %if.then54, %for.end52
  ret void
}

; Function Attrs: inlinehint nounwind
define linkonce_odr dereferenceable(4) i32* @_ZSt3minIiERKT_S2_S2_(i32* dereferenceable(4) %__a, i32* dereferenceable(4) %__b) #5 {
entry:
  %retval = alloca i32*, align 4
  %__a.addr = alloca i32*, align 4
  %__b.addr = alloca i32*, align 4
  store i32* %__a, i32** %__a.addr, align 4
  store i32* %__b, i32** %__b.addr, align 4
  %0 = load i32** %__b.addr, align 4
  %1 = load i32* %0, align 4
  %2 = load i32** %__a.addr, align 4
  %3 = load i32* %2, align 4
  %cmp = icmp slt i32 %1, %3
  br i1 %cmp, label %if.then, label %if.end

if.then:                                          ; preds = %entry
  %4 = load i32** %__b.addr, align 4
  store i32* %4, i32** %retval
  br label %return

if.end:                                           ; preds = %entry
  %5 = load i32** %__a.addr, align 4
  store i32* %5, i32** %retval
  br label %return

return:                                           ; preds = %if.end, %if.then
  %6 = load i32** %retval
  ret i32* %6
}

; Function Attrs: nounwind
declare void @srand(i32) #2

; Function Attrs: nounwind
declare i32 @time(i32*) #2

; Function Attrs: nounwind
declare i32 @rand() #2

declare dereferenceable(140) %"class.std::basic_ostream"* @_ZStlsISt11char_traitsIcEERSt13basic_ostreamIcT_ES5_PKc(%"class.std::basic_ostream"* dereferenceable(140), i8*) #0

declare dereferenceable(140) %"class.std::basic_ostream"* @_ZNSolsEi(%"class.std::basic_ostream"*, i32) #0

define internal void @_GLOBAL__sub_I_gups_fgl.cpp() section ".text.startup" {
entry:
  call void @__cxx_global_var_init()
  ret void
}

attributes #0 = { "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "stack-protector-buffer-size"="8" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #1 = { nounwind }
attributes #2 = { nounwind "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "stack-protector-buffer-size"="8" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #3 = { nounwind readonly "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "stack-protector-buffer-size"="8" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #4 = { noreturn nounwind "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "stack-protector-buffer-size"="8" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #5 = { inlinehint nounwind "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "stack-protector-buffer-size"="8" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #6 = { nounwind readonly }
attributes #7 = { noreturn nounwind }

!llvm.ident = !{!0}

!0 = metadata !{metadata !"clang version 3.5.0 (branches/release_35 225468)"}
