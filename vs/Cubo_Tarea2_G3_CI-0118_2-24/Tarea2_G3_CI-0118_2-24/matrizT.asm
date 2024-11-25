.DATA
    oneF dd 1.0 

.CODE

translateMatrix PROC ; (float* matrixPtr, float* translationVectorPtr, float* resultMatrixPtr)
    ; RCX -> Direcci?n de la matriz de entrada
    ; RDX -> Direcci?n del vector de traslaci?n (tx, ty, tz)
    ; R8  -> Direcci?n de la matriz resultado
    push RAX
    movss xmm0, dword ptr [RDX]        ; tx
    movss xmm1, dword ptr [RDX + 4]    ; ty
    movss xmm2, dword ptr [RDX + 8]    ; tz

    ; Primera fila
    movups xmm3, xmmword ptr [RCX]     
    movups xmmword ptr [R8], xmm3      
    movss dword ptr [R8 + 12], xmm0    

    ; Segunda fila
    movups xmm3, xmmword ptr [RCX + 16] 
    movups xmmword ptr [R8 + 16], xmm3  
    movss dword ptr [R8 + 28], xmm1     

    ; Tercera fila
    movups xmm3, xmmword ptr [RCX + 32] 
    movups xmmword ptr [R8 + 32], xmm3  
    movss dword ptr [R8 + 44], xmm2     

    ; Copiar la cuarta fila
    movups xmm3, xmmword ptr [RCX + 48] 
    movups xmmword ptr [R8 + 48], xmm3
    ; 1 en la cuarta entrada diagonal
    mov EAX, oneF
    mov dword ptr [R8 + 60], EAX
    
    pop RAX
    ret
translateMatrix ENDP

scaleMatrix PROC ; (float* matrixPtr, float* translationVectorPtr, float* resultMatrixPtr)
    ; RCX -> Direcci?n de la matriz de entrada
    ; RDX -> Direcci?n del vector de escala (sx, sy, sz)
    ; R8  -> Direcci?n de la matriz resultado
    push RAX
    movss xmm0, dword ptr [RDX]        ; sx
    movss xmm1, dword ptr [RDX + 4]    ; sy
    movss xmm2, dword ptr [RDX + 8]    ; sz

    ; Primera fila
    movups xmm3, xmmword ptr [RCX]
    movups xmmword ptr [R8], xmm3
    ; primera entrada diagonal
    movss dword ptr [R8 + 0], xmm0    

    ; Segunda fila
    movups xmm3, xmmword ptr [RCX + 16] 
    movups xmmword ptr [R8 + 16], xmm3
    ; segunda entrada diagonal
    movss dword ptr [R8 + 20], xmm1

    ; Tercera fila
    movups xmm3, xmmword ptr [RCX + 32]
    movups xmmword ptr [R8 + 32], xmm3
    ; tercera entrada diagonal
    movss dword ptr [R8 + 40], xmm2

    ; Copiar la cuarta fila
    movups xmm3, xmmword ptr [RCX + 48] 
    movups xmmword ptr [R8 + 48], xmm3
    ; 1 en la cuarta entrada diagonal
    mov EAX, oneF
    mov dword ptr [R8 + 60], EAX
    
    pop RAX
    ret
scaleMatrix ENDP

END
