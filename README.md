# Advanced-Programming-Major-assignment
A, Hướng dẫn cài đặt:
1.	Cài đặt Visual Studio Code:
-	Vào trang: https://code.visualstudio.com/, click vào Download for Window
-	Khi tải xuống thành công, tiến hành chạy file cài đặt, liên tục ấn Next – Next – Next là xong.
-	Vào Visual Studio Code (Biểu tượng trên màn hình Desktop).
-	Trong VSC nhấn tổ hợp Ctrl + Shift + X. Tiếp theo gõ trên thanh tìm kiếm từ khóa “C++”, sau đó chọn extension C/C++ (C/C++ IntelliSense, debugging, and code browsing) do Microsoft phát hành và ấn Install để cài đặt.
2.	Cài đặt môi trường:
-	Vào trang https://winlibs.com/.
-	Tại phần Release Version, kéo xuống phần MSVCRT runtime, thanh GCC 11.3.0 + LLVM/Clang/LLD/LLDB 14.0.3 + MinGW-w64 10.0.0 (MSVCRT) - release 3.
-	Click vào Win64: 7-Zip archive* ( phần có chứa đầy đủ LLVM/Clang/…) để tải xuống.
-	Sau khi thành công tải xuống, tạo một thư mục trong ổ C, copy và paste file tải xuống vào thư mục mới tạo rồi giải nén.
-	Giải nén xong, click vào folder mingw64, sau đó click vào folder x86_64-w64-mingw32, ở trong đó có folder bin.
*) Copy đường dẫn:
-	Nhấn tổ hợp Window + S, gõ “Edit the system environment variables”, click vào ứng dụng hiện ra.
-	Trong “Edit the system environment variables”, vào phần Advanced. Ở đó có phần Environment Variables, click vào
-	Trong phần System Variable, click đúp vào PATH. Sau đó, ta copy đường dẫn của file bin vừa tải xuống vào ô trống bất kỳ.
-	Làm tương tự với phần User Variable.
-	Click vào OK, sau đó Apply.
3.	Cài đặt game:
-	Tải xuống folder Version 4.0 trên Github.
•	Vào trang: https://github.com/ZeroGxMax/Advanced-Programming-Major-assignment
•	Click vào phần Code rồi click vào Download Zip.
•	File Advanced-Programming-Major-assignment-main.zip được tải xuống.
•	Tạo một thư mục trong ổ D, copy và paste file tải xuống vào thư mục mới tạo rồi giải nén.
•	Trong folder Advanced-Programming-Major-assignment-main được giải nén có folder Version 4.0 (ta ghi nhớ vị trí của folder này).
-	Trong folder Version 4.0 vừa tải xuống có file bin: 
•	Nhấn tổ hợp Window + S, gõ “Edit the system environment variables”, click vào ứng dụng hiện ra.
•	Trong “Edit the system environment variables”, vào phần Advanced. Ở đó có phần Environment Variables, click vào
•	Trong phần User Variable, click đúp vào PATH. Sau đó, ta copy đường dẫn của file bin vừa tải xuống vào ô trống bất kỳ.
•	Click vào OK, sau đó Apply
-	Tạo file thi hành exe của game:
•	Vào Visual Studio Code, nhấn tổ hợp Ctrl + K +Ctrl + O.
•	Trong cửa sổ hiện ra, tìm kiếm và vào folder Version 4.0 vừa tải xuống từ Github rồi click vào Select Folder.
•	VSC sẽ hiện ra nội dung bên trong folder.
•	Trong folder có file Makefile, click vào nó rồi thay đổi code thành như sau:
all:  
    g++ -I(DIR to Include) -L(DIR to Library) -o main main.cpp -lmingw32 -lSDL2main -lSDL2 -lSDL2_image -lSDL2_ttf -lSDL2_mixer
trong đó DIR to Include và DIR to library là đường dẫn đến folder include và lib trong folder Version 4.0 vừa tải xuống.
•	Click vào Terminal ở thanh trên của VSC, click vào New terminal.
•	Trong Terminal vừa mới hiện ra, gõ lệnh
mingw32-make
khi đó, file main.exe được tạo ra
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
*) Link video: https://youtu.be/KjZKzVcNzyg, 

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

E, Kết luận và tâm đắc rút ra được:
1.	Kết luận:
-	Về mặt Gameplay, trò chơi đã được làm tương đối hoàn thiện (đã làm được tất cả các hàm đặt cược, xúc sắc, hiển thị xúc sắc, hệ thống tiền tệ và hệ thống tiền thưởng hoàn thiện).
-	Về mặt các lỗi của trò chơi: Cho tới hiện tại, chưa tìm ra lỗi lớn nào cả.
-	Về mặt hiệu năng: Trung bình, game chạy ổn, không quá lag.
-	Về mặt đồ họa:
•	Đồ họa game trung bình kém, các hình ảnh trong game còn sơ sài, chắp vá, thiếu ăn ý với nhau.
•	Khuyết thiếu hình ảnh động (Xúc sắc đứng yên một chỗ)
•	Hệ thống tiền tệ chỉ có thể render số (nếu là hình các chip sẽ đẹp hơn).
-	Về mặt âm thanh:
•	Âm nhạc trong game đã đầy đủ, tuy nhiên nên cho thêm người chơi lựa chọn âm nhạc
•	Hệ thống SFX còn thiếu thốn (Tốt nhất là khi lăn xúc sắc xong, có SFX đọc số các xúc sắc và tổng)
-	Về mặt độ thân thiện người dùng:
•	Game tương đối đơn giản, dễ chơi (miễn là biết được luật)
•	Tuy nhiên, nên tăng thêm chức năng đánh giá số tiền kiếm được sau khi đặt vào một vị trí nào đó (Để cho người chơi dễ tính toán).
-	Về mặt độ thu hút trò chơi:
•	Trò chơi có chút nhàm chán, lặp lại (chỉ có đặt cược – nhận tiền – đặt cược)
•	Có thể thêm chức năng thử thách (ví dụ yêu cầu đặt cược ô xác định và kiếm được bao nhiêu tiền), chức năng shop vật phẩm, chức năng chơi với máy (máy chọn ngẫu nhiên một ô và cược tiền).
2.	Tâm đắc:
-	Handle button event: Tạo ra hàm struct LButton và tạo hàm isInside() để kiểm soát nó. Đồng thời, có thể sử dụng SDL_Rect để chia khu vực cho Button
-	Chia trò chơi thành từng giai đoạn:
•	Dùng Enum để chia giai đoạn cho trò chơi
•	Dùng lệnh if để kiểm soát từng giai đoạn của trò chơi và các hàm bên trong nó
-	Render Text: Sử dụng TTF để render Text, muốn render Text có số bất kỳ, phải trải qua một loạt thao tác
string message = "Total: ";
    char total[3];
    sprintf(total, "%d", TotalOfThreeDice);
    string sTotal = string(total);
    sTotal = message + sTotal;
    gTextTotal.loadFromRenderedText(sTotal.c_str(), BLACK_COLOR);
    gTextTotal.render(totalTextPos.x, totalTextPos.y, NULL);
    SDL_RenderPresent(gRenderer);
3.	Hướng phát triển:
-	Cải tiến đồ họa: Hệ thống tiền tệ dùng các chips, thêm hình ảnh động cho xúc sắc
-	Cải tiến chức năng trò chơi: Chức năng đánh giá số tiền kiếm được sau khi đặt vào một vị trí nào đó, chức năng thử thách, chức năng AI, chức năng shop, chức năng vật phẩm/thời trang.
-	Cải tiến âm thanh trò chơi: Có SFX đọc số các xúc sắc và tổng, nhiều bản nhạc hơn và cho phép người chơi chọn.
