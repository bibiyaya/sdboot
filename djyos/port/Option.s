@ cpu revision definition s3c6410 has evt0, evt1
	.equ    evt0,                36410100
	.equ    evt1,                36410101
	.equ    cpu_revision,        evt1		@时钟设置是没考虑evt0，只能选择 evt1

@ system clock definition
	.equ    clk_133MHZ,          133000000
	.equ    clk_200MHZ,          200000000
	.equ    clk_266MHZ,          266000000
	.equ    clk_300MHZ,          300000000
	.equ    clk_400MHZ,          400000000
	.equ    clk_532MHZ,          532000000
	.equ    clk_666MHZ,          666000000        @ async

	.equ    eclk_96MHZ,          96000000
	.equ    eclk_84_7MHZ,        84666667    @ for iis 44.1 khz
	.equ    eclk_92MHZ,          92160000    @ for iis 48 khz

	.equ    syncmode,            1

@ change apll_clk definition
@	 .equ  apll_clk,      clk_133MHZ        @ sync 133:133:66.5
@	 .equ  apll_clk,      clk_200MHZ        @ sync 200:100:50
@	 .equ  apll_clk,      clk_266MHZ        @ sync 266:133:66.5
@	 .equ  apll_clk,      clk_300MHZ        @ sync 400:75:25
@	 .equ  apll_clk,      clk_400MHZ        @ sync 400:100:50
	.equ    apll_clk,      clk_532MHZ        @ sync 532:133:66.5
@	 .equ  apll_clk,      clk_666MHZ        @ sync 666:133.4:66.5

.equ    fin,                 12000000

@ fout = mdiv*fin/(pdiv*2^sdiv)
@ fvco = mdiv*fin/pdiv

@ change eclk definition for epll fout
@	 .equ    eclk,        eclk_96MHZ
	.equ    eclk,        eclk_84_7MHZ
@	 .equ    eclk,        eclk_92MHZ

@ set clock source : mpll, apll

@ mpll setting,mclk use for asyncronous clk mode
    .equ    mpll_mdiv,          266       @ fvco=1064MHZ, fout=266MHZ
    .equ    mpll_pdiv,          3
    .equ    mpll_sdiv,          2
    .equ    mpll_clk,           (((fin>>mpll_sdiv)/mpll_pdiv)*mpll_mdiv)  @ mpll clock

@ apll setting

.if (apll_clk == clk_133MHZ)
    .equ    apll_mdiv,          133    @ fvco=532MHZ, fout=133MHZ
    .equ    apll_pdiv,          3
    .equ    apll_sdiv,          2
.endif
.if (apll_clk == clk_200MHZ)
    .equ    apll_mdiv,          200    @ fvco=800MHZ, fout=200MHZ
    .equ    apll_pdiv,          3
    .equ    apll_sdiv,          2
.endif
.if (apll_clk == clk_266MHZ)
    .equ    apll_mdiv,          266    @ fvco=1064MHZ, fout=266MHZ
    .equ    apll_pdiv,          3
    .equ    apll_sdiv,          2
.endif
.if (apll_clk == clk_300MHZ)
    .equ    apll_mdiv,          300    @ fvco=600MHZ, fout=300MHZ
    .equ    apll_pdiv,          6
    .equ    apll_sdiv,          1
.endif
.if (apll_clk == clk_400MHZ)
    .equ    apll_mdiv,          400    @ fvco=800MHZ, fout=400MHZ
    .equ    apll_pdiv,          6
    .equ    apll_sdiv,          1
.endif
.if (apll_clk == clk_532MHZ)
    .equ    apll_mdiv,          266    @ fvco=1064MHZ, fout=532MHZ
    .equ    apll_pdiv,          3
    .equ    apll_sdiv,          1
.endif
.if (apll_clk == clk_666MHZ)
    .equ    apll_mdiv,          333    @ fvco=1332MHZ, fout=666MHZ
    .equ    apll_pdiv,          3
    .equ    apll_sdiv,          1
.endif

@ set clock dividers

    .equ    mpll_div,           1       @ dout_mpll = mpll_fout/2 = 133M
    .equ    apll_div,   		0       @ aclk = apll_clk
    .equ    hclk_div,           1       @ ahb_clk = hclkx2/2 = 133M
    .equ    pclk_div,           3       @ pclk = hclkx2/4 = 66.5M

    .equ    aclk,               (apll_clk/(apll_div+1))
    .equ    mclk,               (mpll_clk/(mpll_div+1))
    .equ    pclk,               (hclkx2/(pclk_div+1))

.if (syncmode == 1)   @ use apll as memory clock source
    .if (apll_clk > clk_532MHZ)
        .equ    hclkx2_div,     2       @ hclkx2 = apll_clk/3
    .else
	    .if ((apll_clk > clk_266MHZ) && (apll_clk <= clk_532MHZ))
	        @ arm:ahb:apb = 12:2:1, hclkx2 = apll_clk/3
	        .equ    hclkx2_div, 1       @ hclkx2 = apll_clk/2
	    .else
	        .equ    hclkx2_div,	0       @ hclkx2 = apll_clk/1
	    .endif
    .endif
    .equ    hclkx2,             (apll_clk/(hclkx2_div+1))
.else ; use mpll as memory clock source
    .equ    hclkx2_div,         0       @ hclkx2 = mpll_clk
    .equ    hclkx2,             (mpll_clk/(hclkx2_div+1))
.endif  @for (syncmode == 1)


    .equ    hclk,               (hclkx2/(hclk_div+1))

.if (eclk == eclk_96MHZ)    @ epll fout 96 MHZ
    .equ    epll_mdiv,          32
    .equ    epll_pdiv,          1
    .equ    epll_sdiv,          2
    .equ    epll_kdiv,          0
.endif    @ for (eclk == eclk_96MHZ)

.if (eclk == eclk_84_7MHZ)    @ epll fout 84.666667 MHZ
    .equ    epll_mdiv,          254
    .equ    epll_pdiv,          9
    .equ    epll_sdiv,          2
    .equ    epll_kdiv,          0
.endif    @ for (eclk == eclk_84_7MHZ)

.if (eclk == eclk_92MHZ)    @ epll fout 92.16  MHZ
    .equ    epll_mdiv,          192
    .equ    epll_pdiv,          25
    .equ    epll_sdiv,          0
    .equ    epll_kdiv,          0
.endif    @ for (eclk == eclk_92MHZ)

    .macro mov_pc_lr
      .ifdef thumbcode
            bx      lr
      .else

            mov     pc,lr
      .endif
    .endm

    .macro moveq_pc_lr
      .ifdef thumbcode
            bxeq      lr
      .else

            moveq     pc,lr
      .endif
    .endm

