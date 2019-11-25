GLOBAL(rmt_memcpy)
  pushw     %si
  pushw   %di
  movw    %ax, %di
  movw    %dx, %si
  pushw   %cx
  shrw    $2, %cx
  rep; movsl
  popw    %cx
  andw    $3, %cx
  rep; movsb
  popw    %di
  popw    %si
  retl
ENDPROC(rmt_memcpy)
