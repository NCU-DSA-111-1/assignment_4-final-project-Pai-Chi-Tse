
棋盤格式
------
x軸座標由左自右依序為0~3
y軸座標由上到下依序為0~3
0~3th floor 為立體四子棋的最底層到最上層，即z軸座標0~3

下棋
------
首先輸入欲下位置的x座標，按enter之後，輸入欲下位置的y座標<br>
輸入-1可以退出遊戲<br>
輸入完畢後，會顯示更新後得棋盤，紅棋為我方，藍棋為電腦<br>

輸贏判斷
-------
若任一方完成連線即可獲勝，遊戲過程會被存入txt檔<br>
連線的可能有:<br>
1.平行x或y或z軸的線(各有16種可能)<br>
2.在xy、yz、xz平面上的斜直線(各有8種可能)<br>
3.(0,0,0)(3,3,3)、(0,0,3)(3,3,0)、(0,3,0)(3,0,3)、(0,3,3)(3,0,0)兩點之間的對角線(共四組)<br>
