;*********************************************************************************************************
;                                              KingSky
;                                         ʵʱ����ϵͳ�ں�
;
;                                    (c) Copyright 2011��ׯͩȪ
;                                         All Rights Reserved
;
;                                               ARM920T Port
;                                            ADS v1.2 Compiler
;                                            Samsung S3C2440A
;
; �ļ�:   : os_cpu_a.s 
; CPU :   : S3C2440
; History : 
;  OSCtxSw(), OSIntCtxSw()  OSStartHighRdy() 
;******************************************************************************************************** */

$NOMOD51
EA	BIT	    0A8H.7
SP	DATA	081H
B	DATA	0F0H
ACC	DATA	0E0H
DPH	DATA	083H
DPL	DATA	082H
PSW	DATA	0D0H
TR0	BIT	    088H.4
TH0	DATA	08CH
TL0	DATA	08AH

NAME KS_CPU_A    ;ģ����
;�����ض�λ��
?PR?ks_start_high?KS_CPU_A    SEGMENT CODE
?PR?ks_thread_sw?KS_CPU_A     SEGMENT CODE
?PR?ks_int_sw?KS_CPU_A        SEGMENT CODE

;��������ȫ�ֱ������ⲿ�ӳ���
        EXTRN DATA  (?C_XBP)     ;�����ջָ����������ֲ���������,ΪV2.51�ܱ�Cʹ�ö����ڱ�ģ����

        EXTRN IDATA (current_thread)
        EXTRN IDATA (high_thread)
        EXTRN IDATA (ks_running)						   					   
    	EXTRN IDATA (int_nesting)	
						 

 ;��������4���������뺯��
        PUBLIC ks_start_high
        PUBLIC ks_thread_sw
        PUBLIC ks_int_sw
		;�����ջ�ռ䡣ֻ���Ĵ�С����ջ�����keil������ͨ����ſ��Ի��keil�����SP��㡣

?STACK SEGMENT IDATA
        RSEG ?STACK
OSStack:
        DS 40H
OSStkStart IDATA OSStack-1

PUSHALL    MACRO	;����ѹջ��ջ��
        PUSH ACC
        PUSH B
        PUSH DPH
        PUSH DPL
        PUSH PSW
        MOV  A,R0   ;R0-R7��ջ
        PUSH ACC
        MOV  A,R1
        PUSH ACC
        MOV  A,R2
        PUSH ACC
        MOV  A,R3
        PUSH ACC
        MOV  A,R4
        PUSH ACC
        MOV  A,R5
        PUSH ACC
        MOV  A,R6
        PUSH ACC
        MOV  A,R7
        PUSH ACC
        ;PUSH SP    ;���ر���SP�������л�ʱ����Ӧ�������
        ENDM
    
POPALL    MACRO
        ;POP  ACC   ;���ر���SP�������л�ʱ����Ӧ�������
        POP  ACC    ;R0-R7��ջ
        MOV  R7,A
        POP  ACC
        MOV  R6,A
        POP  ACC
        MOV  R5,A
        POP  ACC
        MOV  R4,A
        POP  ACC
        MOV  R3,A
        POP  ACC
        MOV  R2,A
        POP  ACC
        MOV  R1,A
        POP  ACC
        MOV  R0,A
        POP  PSW
        POP  DPL
        POP  DPH
        POP  B
        POP  ACC
        ENDM
;�ӳ���
;-------------------------------------------------------------------------
        RSEG ?PR?ks_start_high?KS_CPU_A
ks_start_high:
        USING 0    ;�ϵ��51�Զ����жϣ��˴�������CLR EAָ���Ϊ���˴���δ���жϣ��������˳��󣬿��жϡ�

OSCtxSw_in:
    
        ;OSTCBCur ===> DPTR  ��õ�ǰTCBָ�룬���C51.PDF��178ҳ
        MOV  R0,#LOW (current_thread) ;���OSTCBCurָ��͵�ַ��ָ��ռ3�ֽڡ�+0����+1��8λ����+2��8λ����
        INC  R0
        MOV  DPH,@R0    ;ȫ�ֱ���OSTCBCur��IDATA��
        INC  R0
        MOV  DPL,@R0
    
        ;OSTCBCur->OSTCBStkPtr ===> DPTR  ����û���ջָ��
        INC  DPTR        ;ָ��ռ3�ֽڡ�+0����+1��8λ����+2��8λ����
        MOVX A,@DPTR     ;.OSTCBStkPtr��voidָ��
        MOV  R0,A
        INC  DPTR
        MOVX A,@DPTR
        MOV  R1,A
        MOV  DPH,R0
        MOV  DPL,R1
    
        ;*UserStkPtr ===> R5  �û���ջ��ʼ��ַ����(���û���ջ���ȷ��ڴ˴�)  ����ĵ�˵��  ָ���÷����C51.PDF��178ҳ    
        MOVX A,@DPTR     ;�û���ջ����unsigned char��������
        MOV  R5,A        ;R5=�û���ջ����
    
        ;�ָ��ֳ���ջ����
        MOV  R0,#OSStkStart
        
restore_stack:
    
        INC  DPTR
        INC  R0
        MOVX A,@DPTR
        MOV  @R0,A
        DJNZ R5,restore_stack
    
        ;�ָ���ջָ��SP
        MOV  SP,R0
    
        ;�ָ������ջָ��?C_XBP        
        INC  DPTR
        MOVX A,@DPTR
        MOV  ?C_XBP,A    ;?C_XBP �����ջָ���8λ
        INC  DPTR
        MOVX A,@DPTR
        MOV  ?C_XBP+1,A  ;?C_XBP �����ջָ���8λ
    
        ;OSRunning=TRUE
        MOV  R0,#LOW (ks_running)
        MOV  @R0,#01
    
        POPALL
        SETB EA    ;���ж�
        RETI
		
		;-------------------------------------------------------------------------
        RSEG ?PR?ks_thread_sw?KS_CPU_A
ks_thread_sw:    
        PUSHALL
    
OSIntCtxSw_in:
    
        ;��ö�ջ���Ⱥ���ַ
        MOV  A,SP
        CLR  C
        SUBB A,#OSStkStart
        MOV  R5,A     ;��ö�ջ����        
    
        ;OSTCBCur ===> DPTR  ��õ�ǰTCBָ�룬���C51.PDF��178ҳ
        MOV  R0,#LOW (current_thread) ;���OSTCBCurָ��͵�ַ��ָ��ռ3�ֽڡ�+0����+1��8λ����+2��8λ����
        INC  R0
        MOV  DPH,@R0    ;ȫ�ֱ���OSTCBCur��IDATA��
        INC  R0
        MOV  DPL,@R0
    
        ;OSTCBCur->OSTCBStkPtr ===> DPTR  ����û���ջָ��
        INC  DPTR        ;ָ��ռ3�ֽڡ�+0����+1��8λ����+2��8λ����
        MOVX A,@DPTR     ;.OSTCBStkPtr��voidָ��
        MOV  R0,A
        INC  DPTR
        MOVX A,@DPTR
        MOV  R1,A
        MOV  DPH,R0
        MOV  DPL,R1
        
        ;�����ջ����
        MOV  A,R5
        MOVX @DPTR,A
    
        MOV  R0,#OSStkStart  ;��ö�ջ��ַ
save_stack:
    
        INC  DPTR
        INC  R0
        MOV  A,@R0
        MOVX @DPTR,A
        DJNZ R5,save_stack
        
        ;��������ջָ��?C_XBP
        INC  DPTR
        MOV  A,?C_XBP    ;?C_XBP �����ջָ���8λ
        MOVX @DPTR,A
        INC  DPTR
        MOV  A,?C_XBP+1  ;?C_XBP �����ջָ���8λ
        MOVX @DPTR,A        
    
        
        ;OSTCBCur = OSTCBHighRdy
        MOV  R0,#current_thread
		MOV  R1,#high_thread
		MOV  A,@R1
        MOV  @R0,A
        INC  R0
		INC  R1
		MOV  A,@R1
        MOV  @R0,A
        INC  R0
		INC  R1
		MOV  A,@R1
        MOV  @R0,A
                
        ;OSPrioCur = OSPrioHighRdy  ʹ��������������ҪĿ����Ϊ��ʹָ��Ƚϱ�Ϊ�ֽڱȽϣ��Ա��ʡʱ�䡣
       ; MOV  R0,#OSPrioCur
	;	MOV  R1,#OSPrioHighRdy
	;	MOV  A,@R1
       ; MOV  @R0,A
        
        LJMP OSCtxSw_in	
RSEG ?PR?ks_int_sw?KS_CPU_A
ks_int_sw:

        ;����SPָ��ȥ���ڵ���OSIntExit(),OSIntCtxSw()������ѹ���ջ�Ķ�������
        ;SP=SP-4

        MOV  A,SP
        CLR  C
        SUBB A,#4
        MOV  SP,A
        
        LJMP OSIntCtxSw_in
			
	END
	
	
	


