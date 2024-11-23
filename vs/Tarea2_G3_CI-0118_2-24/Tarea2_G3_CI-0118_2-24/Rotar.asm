.DATA
resultado dd 0.0 

.CODE

translateMatrix PROC ; (float* matrixPtr, float* translationVectorPtr, float* resultMatrixPtr)
    ; RCX -> Dirección de la matriz de entrada
    ; RDX -> Dirección del vector de traslación (tx, ty, tz)
    ; R8  -> Dirección de la matriz resultado

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

    ; Copiar la cuarta fila sin modificaciones
    movups xmm3, xmmword ptr [RCX + 48] 
    movups xmmword ptr [R8 + 48], xmm3  

    ret
translateMatrix ENDP

END
