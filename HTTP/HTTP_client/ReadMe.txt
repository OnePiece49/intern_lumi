                    /********************************* Giao thức HTTP *********************************\
    1, Mô hình Client-Server
            +, Để hiểu được mô hình Client-Server, chúng ta hãy lấy 1 ví dụ nhỏ là: bạn muốn biết thời tiết của thành phố trong hôm nay ?
            +, Để làm điều này, bạn sẽ cần hỏi một người biết về điều kiện thời tiết ở thành phố. Giả sử chưa có máy tính, bạn có thể biết 
        được thông tin thời tiết qua báo, đài. Trong quá trình này, có hai người tham gia riêng biệt. Đầu tiên là bạn, người muốn 
        biết thông tin về thời tiết. Người thứ hai là báo hoặc đài, người cung cấp thông tin về thời tiết. Nếu đặt tên cho hai người
        tham gia này, chúng ta có:
            +, Người tiêu dùng muốn biết một thông tin cụ thể, là người đưa ra yêu cầu, hay còn được gọi là Client trong ngữ cảnh Client-server.
            +, Nhà cung cấp thông tin, là người tiếp nhận yêu cầu và xử lý nó, hay còn gọi là Server trong ngữ cảnh Client-server.
    

    2, Giao thức HTTP là gì?    
        HTTP (HyperText Transfer Protocol – Giao thức truyền tải siêu văn bản) là một trong các giao thức chuẩn về mạng Internet, được dùng
    để liên hệ thông tin giữa Máy cung cấp dịch vụ (Web server) và Máy sử dụng dịch vụ (Web client), là giao thức Client/Server dùng 
    cho World Wide Web – WWW.

        Đối với Client và Server trên Web chúng ta cần phải có một phương thức giao tiếp, cụ thể là một giao thức để hai hệ thống có thể hiểu và
    tương tác với nhau. Giao thức này được gọi là giao thức truyền thông HTTP.
            
            +, Mô hình giao thức:
        • HTTP client thiết lập một kết nối TCP đến server. Nếu thiết lập thành công,
        client và server sẽ truyền nhận dữ liệu với nhau thông qua kết nối này, kết
        nối được thiết lập còn gọi là socket interface bao gồm các thông tin: địa chỉ
        IP, loại giao thức giao vận (chính là TCP), và port (mặc định là 80).
                socket interface = IP address + TCP +Port

        • Sau khi kết nối thành công, client gửi một HTTP request đến server thông
        qua socket interface vừa được thiết lập. Trong gói tin request sẽ chứa đường
        dẫn yêu cầu (path name).
        • Server sẽ nhận và xử lý request từ client thông qua socket, sau đó đóng gói
        dữ liệu tương ứng và gửi một HTTP response về cho client. Dữ liệu trả về
        sẽ là một file HTML chứa loại dữ liệu khác nhau như văn bản, hình ảnh,…
        • Server đóng kết nối TCP.
        • Client nhận được dữ liệu phản hồi từ server và đóng kết nối TCP.


    2, Cách hoạt động giao thức HTTP:
        +, Giao thức HTTP hoạt động dựa trên mô hình Client – Server. Thông thường khi các bạn lướt web, các máy tính của người dùng sẽ đóng 
    vai trò làm máy khách (Client). Sau một thao tác nào đó của người dùng, các máy khách sẽ gửi yêu cầu đến máy chủ (Server) và chờ
    đợi câu trả lời từ những máy chủ này.
        +, Client và Server được kết nối với nhau bằng giao thức HTTP. Khi kết nối được thiết lập, Client sẽ gửi yêu cầu tới Server dưới 
    dạng XML hoặc Json mà cả hai đều có thể hiểu được. Sau khi hiểu yêu cầu, Server sẽ trả về một response (phản hồi) để trả về dữ liệu
    mà Client yêu cầu dưới dạng XML hoặc Json.
        +, Ngoài ra, khi các hệ thống trao đổi dữ liệu với nhau, chúng cũng sử dụng giao thức này nhưng 2 bên đều là server.
    
    3, HTTP – Requests
        +, HTTP request là thông tin được gửi từ client lên server, để yêu cầu server tìm hoặc xử lý một số thông tin, dữ liệu mà client muốn. 
    HTTP request có thể là một file text dưới dạng XML hoặc Json mà cả hai đều có thể hiểu được.
        Một HTTP client (máy khách) gửi một HTTP request (yêu cầu) lên server (máy chủ) nhờ một thông điệp có định dạng như sau:
        <method> <request-URL> <http-serverion>
        <headers>
        <body>
            Request line là dòng đầu tiên của gói HTTP Request, bao gồm 3 trường: phương thức (method), đường dẫn (path – có nhiều bài viết gọi là 
        URL hoặc URI cho trường này) và phiên phản giao thức (HTTP version).
        --> Để gửi các thông tin ta cần gửi một lệnh Http Request.
            Chẳng hạn, để cập nhật số điện thoại khách đến chơi, ta thực hiện lệnh gồm 3 phần chính là: Request line, Header và Body.
        3.1, Các phương thức<method> của HTTP request:
        3.1.1 Phương thức GET và POST:
            GET và POST là hai phương thức của giao thức HTTP, đều là gửi dữ liệu về server xử lí sau khi người dùng nhập thông tin vào
        form và thực hiện submit. Trước khi gửi thông tin, nó sẽ được mã hóa bằng cách sử dụng một giản đồ gọi là url encoding. Giản đồ 
        này là các cặp name/value được kết hợp với các kí hiệu = và các kí hiệu khác nhau được ngăn cách bởi dấu &.
                name=value1&name1=value2&name2=value3
        3.1.1.1: phương thức GET:
            Phương thức GET gửi thông tin người dùng đã được mã hóa thêm vào trên yêu cầu trang:
            Vd: http://www.example.com/index.htm?name=value1&name1=value1
                --> Chúng ta thấy rằng GET lộ thông tin trên đường dẫn URL. Băng thông của nó chỉ khoảng 1024 kí tự vì vây GET hạn chế về số 
            kí tự được gửi đi. GET không thể gửi dữ liệu nhị phân , hình ảnh ... Có thể cached và được bookmark (đánh dấu trên trình duyệt). 
            Lưu trong browser history.
        3.1.1.2: phương thức POST:



            /********************************* THƯ VIỆN LIBCURL *********************************\

    CURLcode curl_easy_setopt(CURL *handle, CURLoption option, parameter);

I. Callback Options

1. CURLOPT_WRITEFUNCTION
    --> function này phải theo mẫu:  size_t function( void *ptr, size_t size, size_t nmemb, void *stream);
    --> function này sẽ được gọi bởi curl ngay lập tức khi nhận được data từ server gửi đến

2. CURLOPT_WRITEDATA
    If you're using libcurl as a win32 DLL, you MUST use the CURLOPT_WRITEFUNCTION if you set this option or you will experience crashes.