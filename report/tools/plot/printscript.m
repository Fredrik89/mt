cd logs/test1





h=v_handle
figure(v_handle)

W = 3; H = 2.25;
set(h,'PaperUnits','inches')
set(h,'PaperOrientation','portrait');
set(h,'PaperSize',[H,W])
set(h,'PaperPosition',[0,0,W,H])

print -dpng vp.png



h=sl_handle
figure(sl_handle)

W = 3; H =2.25 ;
set(h,'PaperUnits','inches')
set(h,'PaperOrientation','portrait');
set(h,'PaperSize',[H,W])
set(h,'PaperPosition',[0,0,W,H])

print -dpng sl.png


h=f_handle
figure(f_handle)

W = 3; H = 2.25;
set(h,'PaperUnits','inches')
set(h,'PaperOrientation','portrait');
set(h,'PaperSize',[H,W])
set(h,'PaperPosition',[0,0,W,H])

print -dpng f.png


cd ../..
