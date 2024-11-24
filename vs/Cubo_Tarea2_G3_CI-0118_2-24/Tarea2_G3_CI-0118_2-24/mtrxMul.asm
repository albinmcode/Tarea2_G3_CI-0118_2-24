; Tarea2_G3_CI-0118_2-24

;/ Matrix by vector multiplying
; | 1 0 0 0 |   |x|
; | 0 1 0 0 | * |y|
; | 0 0 1 0 |   |z|
; | 0 0 0 1 |   |w|
;
; v = (x,y,z,w)
; Steps:
; 1. Multiply first matrix row and vector 1*x + 0*y + 0*z + 0*w
; 2. Repeat 4 times, each result is save in a position inside (f1 * v,f2 * v, f3 * v, f4 * v)
;/

.DATA
    resultado dd 0.0

.CODE

; Mul 4x4 matriz y 4x1 vector columna
mulMatrixVector4x1 PROC ; (float* matrixPtr, float* vectorPtr, float* resultVectPtr)
    push RSI
    push RDI
    push RBX

    mov RSI, 0
    mov RDI, 0
    mov BL, 0
    siguienteFila:
        movups xmm0, xmmword ptr [RCX+RSI] ;Carga fila RSI 
        movups xmm1, xmmword ptr [RDX] ;Carga el vector
    
        mulRowVector: ; producto punto entre fila xmm0[RSI] y vector xmm1
        ; Multiplica fila * vector
        vmulps xmm2, xmm0, xmm1
        ;; Suma todas las entradas del producto en xmm2 = [a, b, c, d] 
        vshufps xmm1, xmm2, xmm2, 238 ; [c, d, a, b]
        vaddps xmm2, xmm2, xmm1      ; [a+c, b+d, a+c, b+d]
        vshufps xmm1, xmm2, xmm2, 1 ; [b+d, a+c, b+d, a+c]
        vaddps xmm2, xmm2, xmm1    ; [a+b+c+d,...]
        
        ; guardar en entrada RDI del vector resutado 4x1
        movss dword ptr [R8+RDI], xmm2
        ; Siguiente fila
        add RSI, 16
        ; Siguiente entrada del vector resultado
        add RDI, 4
        ; 4 iteraciones, 4 filas
        inc BL
        cmp BL, 4
        jl siguienteFila
    
    pop RBX
    pop RDI
    pop RSI
    
    ret
    
mulMatrixVector4x1 ENDP

; Mult 4x4 matriz y 4x4 matriz
mulMatrix4x4 PROC ; (float* matrixAptr, float* matrixBptr, float* matrixResPtr)
    push RSI
    push RDI
    push RBX

    mov RBX, RCX      ; RBX = Direcci?n de matA
    mov RSI, RDX      ; RSI = Direcci?n de matB
    mov RDI, R8       ; RDI = Direcci?n de matResult

    xor R9, R9        ; Reiniciar a 0 R9 = fila actual de matA
    filaLoop:
        ; Cargar fila actual de matA
        movups xmm0, xmmword ptr [RBX + R9]

        xor R10, R10   ; Reiniciar R10 = columna actual de matB
        columnaLoop:
            ; Cargar columna actual de B en xmm1
            movss xmm1, dword ptr [RSI + R10]        ; Elemento fila 1, columna j
            movss xmm2, dword ptr [RSI + R10 + 32]   ; Elemento fila 3, columna j
            unpcklps xmm1, xmm2                      ; Empaquetar los dos primeros elementos
            movss xmm2, dword ptr [RSI + R10 + 16]   ; Elemento fila 2, columna j
            movss xmm3, dword ptr [RSI + R10 + 48]   ; Elemento fila 4, columna j
            unpcklps xmm2, xmm3                      ; Empaquetar los ?ltimos dos elementos
            unpcklps xmm1, xmm2                      ; Completar la columna en xmm1
            
            ; Multiplicar fila de matA con la columna de matB
            vmulps xmm2, xmm0, xmm1

            ; Reducir xmm2 (sumar elementos) para obtener un escalar
            vshufps xmm3, xmm2, xmm2, 238
            vaddps xmm2, xmm2, xmm3
            vshufps xmm3, xmm2, xmm2, 1
            vaddps xmm2, xmm2, xmm3

            ; Guardar el escalar en la posici?n correspondiente de matResult
            mov RCX, R9
            add RCX, R10
            movss dword ptr [RDI + RCX], xmm2

            ; Siguiente columna
            add R10, 4    ; Desplazarse a la siguiente columna en matB
            cmp R10, 16    ; 4 columnas * 16 bytes por columna
            jl columnaLoop

        ; Siguiente fila
        add R9, 16        ; Desplazarse a la siguiente fila en matA
        cmp R9, 64        ; 4 filas * 16 bytes por fila
        jl filaLoop

    pop RBX
    pop RDI
    pop RSI
    ret
mulMatrix4x4 ENDP

END

