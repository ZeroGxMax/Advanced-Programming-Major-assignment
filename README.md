# Advanced-Programming-Major-assignment
A, Hướng dẫn cài đặt:
-	Tải xuống folder Version 4.0 trên Github.
-	Trong folder vừa tải xuống có file bin: Vào “Edit the system environment variables”, phần Advanced, click vào Environment Variables rồi add đường dẫn đến file bin vào phần path.
-	Khởi động game folder Version 4.0 với Visual Studio Code
-	Tạo Make file với code sau:
all:  
    g++ -I<DIR to Include> -L<DIR to Library> -o main main.cpp -lmingw32 -lSDL2main -lSDL2 -lSDL2_image -lSDL2_ttf -lSDL2_mixer
trong đó DIR to Include và DIR to library là đường dẫn đến folder include và library trong folder Version 4.0
-	Gọi lệnh
mingw32-make
trong terminal sẽ tạo file main.exe
-	Vào file main.exe để chơi game.

B, Mô tả chung về trò chơi:
-	Tên trò chơi: Tài Xỉu (Sicbo trong Tiếng Anh)
-	Mô tả: Tài xỉu hay còn gọi là sicbo là trò chơi dân gian nổi tiếng bắt nguồn từ Trung Quốc cổ đại. Đánh Tài xỉu là trò chơi dựa trên 3 viên xí ngầu (hay còn gọi là xúc xắc) - mỗi viên xí ngầu có 6 mặt (1, 2, 3, 4, 5, 6). Người chơi sẽ đặt cược kết quả của xí ngầu trước mỗi lần quay.
-	Các ý tưởng chính:
+, Cách chơi game: 
•	Đặt cọc vào một ô trên bàn cờ Tài Xỉu số lượng tiền nhất định, khi đặt cọc xong bấm vào nút Start. 
•	Xúc sắc sẽ được lăn và tạo ra kết quả.
•	Dựa trên kết quả của xúc sắc và vị trí người chơi đặt cược, máy sẽ phán định thắng thua: Thua sẽ mất số tiền đặt cược, thắng sẽ được cộng tiền dựa trên hệ số đặt cược của vị trí đã chọn.
+, Hệ số tiền thưởng của các ô:
•	Tài, Xỉu, Chẵn, Lẻ: 1:1
•	Các ô bộ ba đồng nhất: 1:180
•	Các ô tổng (4-17): Đã ghi rõ trên bàn cờ
•	Các xúc sắc: 1 xúc sắc trùng là 1:1, 2 xúc sắc trùng là 1:2, 3 xúc sắc trùng là 1:3.
-	Luật chơi: Có thể tham khảo https://sanhrong.info/game-tai-xiu/luat-choi, 
C, Các chức năng đã cài đặt:
1.	Menu:
-	Game có main menu: Click vào Bắt đầu để chơi game, click vào Thoát để thoát game
2.	Âm nhạc:
-	Menu có nhạc của Menu.
-	Game sẽ có nhạc của Game.
-	Hành vi lăn xúc sắc, thắng, thua sẽ phát ra âm thanh SFX.
3.	Bàn cờ Tài Xỉu và các hàm đặt cược:
-	Bàn cờ Tài Xỉu được render.
-	Tài sản của người chơi được render ở bên trái màn hình, số tiền người chơi đặt cọc được render bên trên màn hình.
-	Click vào các ô đặt cược sẽ trừ tài sản và tăng số tiền đặt cọc. Ô được đặt cược sẽ hiện lên màu đỏ. (Chỉ có thể đặt được 1 ô duy nhất)
-	Click vào nút Clear để đặt lại.
-	Click vào nút Start để bắt đầu lăn xúc xắc
4.	Các nút bấm phụ:
-	Nút x1, x2, …, x100: Để khống chế số tiền đặt cọc mỗi lần click: Khi click vào x1 sẽ trở thành x2 và mỗi lần click một ô sẽ cược 2 tiền; Khi click vào x100 sẽ trở thành x1.
-	Nút Music ON/ Music OFF: Click vào để bật tắt Nhạc
-	Nút SFX ON/SFX OFF: Click vào để bật tắt SFX
-	Nút Main Menu: Click vào để về Menu
-	Nút Quit Game: Click vào để thoát game
5.	Lăn xúc sắc:
-	Khi lăn xúc sắc, sẽ render xúc sắc nhiều lần (30 lần).
-	Sau 30 lần sẽ được 3 xúc sắc, cộng tổng lại (Tổng sẽ được hiển thị bên dưới màn hình)
6.	Kết quả:
-	Click vào vị trí bất kỳ trên màn hình để tiếp tục
-	Hiện lên banner thắng thua dựa trên kết quả lăn xúc sắc và vị trí người chơi đặt cược, đồng thời phát ra SFX (dùng hàm checkResult để tính toán).
-	Cập nhật kết quả của trò chơi.
7.	Trò chơi mới:
-	Click vào vị trí bất kỳ trên màn hình để bắt đầu trò chơi mới.
*) Link video:
D, Các kỹ thuật lập trình:
-	Cơ bản: Mảng, con trỏ, kiểu dữ liệu tự định nghĩa (struct, enum), switch, if else
-	Các hàm sinh số ngẫu nhiên (từ 1-6 cho xúc xắc).
-	Đồ họa: 
+, loadMedia(): Sử dụng SDL_image để load các hình ảnh PNG
+, Các hàm render (Menu, bàn cờ Tài Xỉu, buttons, xúc sắc, banner)
+, Các biến LTexture (tự định nghĩa, phát sinh từ SDL_Texture), và LButton (tự định nghĩa, phát sinh từ SDL_Texture và SDL_Rect)
-	Text:
+, Sử dụng SDL_TTF
+, Kỹ thuật render Text dựa trên kết quả bất kỳ
-	Âm nhạc:
+, Sử dụng SDL_Mixer để load nhạc và SFX
+, Dựa trên điều kiện nhất định để phát nhạc
-	Xử lý Event:
+, Xử lý Mouse Event (cụ thể là click)
+, Xử lý Keydown Event

E, Tâm đắc rút ra được:
