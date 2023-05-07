# Contra_SDL2
2 repo trước bị lỗi không push code lên được nên phải tạo repo này
## 1. Mô tả về game
- Game thuộc dòng platformer (hay trò chơi đi cảnh), là một thể loại trò chơi video và thể loại phụ của trò chơi hành động. Trong một platformer game, nhân vật do người chơi điều khiển phải nhảy và leo qua các nền tảng bị treo trong khi tránh chướng ngại vật.
- Sau khi bật game, một màn hình sẽ hiện ra, bấm Play Game để bắt đầu vào chơi, Exit để thoát.
- Di chuyển bằng các nút mũi tên sang trái, sang phải.
- Bấm chuột phải để nhảy, chuột trái để bắn.
- Bấm nút M để tắt nhạc.
- Người chơi ban đầu được cấp cho 3 mạng dự phòng (nghĩa là có tất cả 4 mạng), số mạng dự phòng chính là số trái tim hiển thị ở góc trên bên trái màn hình.
- Kẻ địch thì có 2 dạng, một là chỉ biết di chuyển, hai là đứng im nhưng có thể bắn đạn. Nếu người chơi va chạm với kẻ địch hoặc đạn của địch thì sẽ mất một mạng.
- Ngoài số mạng dự phòng ra, ở thanh màu xanh dương phía trên màn hình còn hiển thị số vàng thu được, số quái đã hạ (Mark), điểm số cao nhất từng chơi (high score) và cuối cùng là thời gian còn lại (Time).
- Khi số vàng thu được đạt đủ 40, người chơi sẽ được cấp cho thêm 1 mạng dự phòng nữa, khi thời gian còn 0, thì game sẽ kết thúc.
- Khi game kết thúc, một menu đơn giản sẽ hiện ra, và cho 2 lựa chọn là chơi tiếp (Play Again) hay thoát (Exit).

## 2. Cách lấy file project về máy
- Cách clone project về máy rất đơn giản, các bạn có thể down file .zip về sau đó giải nén, hoặc có thể tạo 1 folder mới rồi dùng git clone, hoặc đơn giản hơn nũa thì hãy tải git desktop sau đó chép đường link liên kết của project này vào và clone về
- Trong file project đã bao gồm cả các file cần thiết của thư viện SDL2 (có bao gồm SDL2_image, SDL2_mixer, SDL2_ttf) nên rất tiện để các bạn link trực tiếp vào project của mình
- Project được code và build trên Visual Studio 2022, sau khi clone về chỉ cần dùng Local window debugger là game sẽ chạy và tạo file .exe , nếu như các bạn sử dụng IDE khác thì hãy lên mạng và tìm cách build file dành cho IDE đó

## 3. Các kĩ năng áp dụng trong code game
- Đồ họa, âm thanh và text sử dụng thư viện SDL2_image, SDL2_mixer và SDL2_ttf.
- Kỹ thuật tile map, di chuyển map theo nhân vật, random map, vô hạn map.
- Xử lý di chuyển, va chạm map, đạn bắn.
- Xử lý thời gian, tỉ số fps.
