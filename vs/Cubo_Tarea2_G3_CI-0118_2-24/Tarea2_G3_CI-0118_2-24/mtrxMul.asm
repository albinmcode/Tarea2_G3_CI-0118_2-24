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
        vmovups ymm0, ymmword ptr [RCX+RSI] ;Carga fila RSI 
        vmovups ymm1, ymmword ptr [RDX] ;Carga el vector
    
    mulRowVector: ; producto punto entre fila ymm0[RSI] y vector ymm1
        ; Multiplica fila * vector
        vmulps ymm2, ymm0, ymm1
        ; Suma todas las entradas del producto en ymm2
        vperm2f128 ymm1, ymm2, ymm2, 1       ; Mueve los 128 bits superiores de ymm2 a ymm1
        vaddps ymm0, ymm0, ymm1              ; Suma los 128 bits superiores e inferiores de ymm0
        vhaddps ymm2, ymm2, ymm2             ; Suma horizontalmente los pares de elementos
        vhaddps ymm2, ymm2, ymm2             ; Suma horizontalmente nuevamente
        ; guardar resultado en variable para ajustar tama?o
        vmovss resultado, xmm2
        ; guardar en entrada RDI del vector resutado 4x1
        mov RAX, 0
        mov EAX, resultado
        mov [R8+RDI], EAX
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

