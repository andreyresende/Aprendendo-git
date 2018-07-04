.text
	li t1,0xFF000000 #Endereco inicial(Primeiro pixel)
	li t2,0x3E #Codigo da cor
	sb t2,0(t1)