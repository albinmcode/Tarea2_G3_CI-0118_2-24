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

END

