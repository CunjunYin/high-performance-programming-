# CITS3402 High Performance Computing
parallel programming
for mac os that unable to run openMP do folowing steps:

1: install brew by terminal CMD:/usr/bin/ruby -e "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/master/install)"

2: install gcc6 CMD: brew reinstall gcc6 --without-multilib

3 open MP lib CMD: brew install llvm

4: gcc-6 -fopenmp -o progm progm.c

VM Linux GCC install process:

1.sudo dpkg --configure -a

2. sudo apt install gcc




Public Function test(ByVal row As Range, ByVal queens As Integer)
    Dim i As Integer
    Dim j As Integer
    Dim count As Integer
    Dim dx As Integer
    Dim dy As Integer
    
    
    For i = 1 To queens
        For j = 1 To queens
            If i <> j Then
                dx = Abs(i - j)
                dy = Abs(row.Cells(1, i).Value - row.Cells(1, j).Value)
                If dx = dy Then
                    count = count + 1
                End If
            End If
        Next j
    Next i
    test = count

End Function

Public Function test1(ByVal row As Range, ByVal j As Integer)
    Dim ret As Integer
    
    If row.Cells(1, j) = j Then
        ret = 1
    Else
        ret = 0
    
    test1 = ret
End Function
