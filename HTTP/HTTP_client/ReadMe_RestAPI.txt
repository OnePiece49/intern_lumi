        /********************************* Design Rest API *********************************\

    API: là giao diện lập trình ứng dụng - thông thường nó là các URL hoặc các function, giúp các Application giao tiếp được với nhau.
        Giao tiếp: sử dụng các chức năng của nhau hoặc trao đổi các thông tin với nhau
        API chia làm 3 loại: Web API, API on OS, API of lib and framework
        --> Web API chia làm 4 loại:
            +, Rest --> Phổ biến nhất
            +, SOAP
            +, XML-RPC
            +, json-RPC
    Rest: là những ràng buộc, là những quy tắc --> Nếu đã áp dụng đầy đủ các quy tắc này thì ta sẽ có 1 hệ thông Rest
    
    +, Các ràng buộc của Rest(constraint):
        +, client-server
        +, Stateless
        +, Cacheable
        +, Uniform Interface
        +, Layer system
        - Code on command
    
    1. client-server
        --> Để có 1 hệ thống Rest, ta cần có 1 hệ thống theo mô hình Client-Server
    2. Stateless
        --> Nghĩa là Server sẽ không lưu thông tin Client gửi tới, mỗi Request sẽ được xử lí độc lập mà ko quan tâm đến Request trước.
        Nên trong mỗi Request, client cần gửi nhiều thông tin hơn.
    3. Cacheable:
        --> Lưu trữ lại thông tin vào mộ nhớ đệm trong thiết bị Client.
            --> Các thông tin lưu trữ lại là thông tin Server response, mà các thông tin này phải được Server cho phép lưu trữ
    4. Uniform Interface
        --> Ta phải thiết kế Url 1 cách thông nhất
        Vd: Mỗi 1 tài nguyên chỉ có 1 Url chỉ tới nó
    5. Layer system
        --> Vd: CHương trình web thì nằm trên svA, mà dữ liệu nằm trên svA thì svA tiếp tục request tới svB, và svB phản hồi về svA, svA gửi
        thông tin về client.



    Có thể nói bản thân REST không phải là một loại công nghệ. Nó là phương thức tạo API với nguyên lý tổ chức nhất định. Những nguyên 
lý này nhằm hướng  dẫn lập trình viên tạo môi trường xử lý API request được toàn diện.
Để hiểu rõ hơn về RESTful API ta sẽ đi lần lượt giải thích các khái niệm API, REST hay RESTful.

1. RESTful API là gì?
    RESTful API là một tiêu chuẩn dùng trong việc thiết kế API cho các ứng dụng web(thiết kế Web services) để tiện cho việc quản lý các resource. 
Nó chú trọng vào tài nguyên hệ thống (tệp văn bản, ảnh, âm thanh, video, hoặc dữ liệu động…), bao gồm các trạng thái tài nguyên được định dạng
và được truyền tải qua HTTP.

2. Diễn giải các thành phần
    API (Application Programming Interface) là một tập các quy tắc và cơ chế mà theo đó, một ứng dụng hay một thành phần sẽ tương tác với một
ứng dụng hay thành phần khác. API có thể trả về dữ liệu mà bạn cần cho ứng dụng của mình ở những kiểu dữ liệu phổ biến như JSON hay XML.

    REST (REpresentational State Transfer) là một dạng chuyển đổi cấu trúc dữ liệu, một kiểu kiến trúc để viết API. Nó sử dụng phương thức HTTP
đơn giản để tạo cho giao tiếp giữa các máy. Vì vậy, thay vì sử dụng một URL cho việc xử lý một số thông tin người dùng, REST gửi một yêu cầu
HTTP như GET, POST, DELETE, vv đến một URL để xử lý dữ liệu.

    RESTful API là một tiêu chuẩn dùng trong việc thiết kế các API cho các ứng dụng web để quản lý các resource. RESTful là một trong những kiểu 
thiết kế API được sử dụng phổ biến ngày nay để cho các ứng dụng (web, mobile…) khác nhau giao tiếp với nhau.

    Chức năng quan trọng nhất của REST là quy định cách sử dụng các HTTP method (như GET, POST, PUT, DELETE…) và cách định dạng các URL cho ứng
dụng web để quản các resource. RESTful không quy định logic code ứng dụng và không giới hạn bởi ngôn ngữ lập trình ứng dụng, bất kỳ ngôn ngữ
hoặc framework nào cũng có thể sử dụng để thiết kế một RESTful API.