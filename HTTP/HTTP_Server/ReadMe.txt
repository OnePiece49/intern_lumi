                /*************** HTTP ***************\
I. Event-Driven
--> Là kiến trúc hướng sự kiện
--> là mô hình chủ yếu được sử dụng trong các ứng dụng tập trung vào việc đưa ra các xử lý phản hồi lại các thao tác của người dùng:
    +, Click vào button trên màn hình
    +, Di chuột
    +, Nhận messages từ chương trình hoặc threads khác
--> Trong một ứng dụng hướng sự kiện, thường có một vòng lặp chính (main loop) lắng nghe các sự kiện, và sau đó kích hoạt lời gọi đến các 
callback function (*) khi phát hiện một sự kiện nào đó xảy ra.

II. Thread-pool:
--> Thread được sinh ra để thực hiện một nhiệm vụ cụ thể, nhiều Thread cùng xử lý công việc giúp chúng ta giải quyết được bài toán thời gian 
và hiệu năng khi xử lý một tác vụ nào đó.
 ??? Bài toán đặt ra ở đây là có phải cứ sinh ra nhiều Thread thì tác vụ của chúng ta sẽ nhanh hơn mượt mà hơn?
-->Câu trả lời là không phải cứ tạo nhiều Thread cùng hoạt động thì sẽ đem lại hiệu năng cao vì mỗi khi có một Thread mới được tạo ra và được cấp p
hát bộ nhớ bằng từ hóa new thì sẽ có vấn đề bộ nhớ và hiệu suất -> có thể dẫn tới crash chương trình.
 
--> Giải quyết bài toán đó ThreadPool ra đời để giới hạn số lượng Thread được chạy bên trong ứng dụng chúng ta cùng một thời điểm.
    Bên trong ThreadPool, các nhiệm vụ sẽ được chèn vào trong một Blocking Queue. Blocking Queue có thể hiểu là nơi chứa các nhiệm vụ mà các Thread
sẽ lấy chúng ra và thực thi lần lượt. Mỗi khi có một nhiệm vụ mới được thêm vào Queue và sau đó sẽ chỉ có một Thread đang không phải thực hiện một nhiệm 
vụ nào vào Queue lấy nhiệm vụ đó ra, còn các Thread còn lại phải chờ đợi cho đến khi Thread đó lấy nhiệm vụ ra thành công.

III. Asynchronous programming và Synchronous programming
Ta có ví dụ:
    +, Giả sử ta có 3 công việc cần thời gian thực hiện t1, t2, t3 giây.
    Khi công việc t1 bắt đầu thực hiện thì t2 phải đợi t1 kết thúc để bắt đầu thực thi, tương tự với t3 cũng vậy
        --> Mô hình lập trình đó là mô hình đồng bộ "Synchronous programming". 

--> Mô hình bất đồng bộ "Asynchronous programming":
    +, Cũng giả sự ta có 3 công việc cần thời gian thực hiện t1, t2, t3 giây.
    --> Nhưng với mô hình bất đồng bộ thì khi công việc t1 thực hiện thì t2 và t3 có thể được thực hiện luôn
        --> Thời gian xử lí công việc nhanh hơn và ta cũng có thể xử lý nhiều công việc cùng một lúc.
    
    +, Trong lập trình socket, mô hình bất đồng bộ(Asynchronous programming) được sử dụng cho cả Client và Server.
        --> Đối với Server, mô hình này giúp Server cho phép xử lí nhiều client và phát huy tốt khả năng xử lí xong xong của Client.
        --> Đối với Client, mô hình này giúp Client không bị treo giao diện khi thực hiện các nhiệm vụ kéo dài.

IV. Blocking và Non-Blocking:
    --> Khi Client gửi một request tới Server thì bên Server sẽ  xử lý request và gửi lại response cho Client. 
Để có thể thực hiện được điều này thì cả Server và Client phải kết nối tới một thành phần trung gian gọi là Socket. Khi cả client và server
tạo mối liên kết tới Socket thì khi đó server sẽ lắng nghe thông qua socket để có thể tiếp nhận request từ client.
    Tóm lại: Sau khi kết nối được tạo ra lúc này cả server và client để đọc và ghi dữ liệu thông qua socket mà đã được liên kết đó.

1. Blocking IO:
    +, Khi một client gửi một request tới server thì khi đó luồng xử lý dữ liệu cho liên kết giữa client và server sẽ bị khóa lại cho đến khi nào 
request đó được thực hiện xong hoàn toàn. Trong thời gian này nếu có một request khác được gửi đến server thì bắt buộc phải chờ cho đến khi tác vụ 
trước đó được hoàn thành.
--> Blocking IO là có nghĩa là xử lý đơn luồng. Thông thường một thao tác máy tính chỉ xảy ra khi các thao tác trước đó đã hoàn thành. Một ví 
dụ điểm hình cho Blocking IO là xử lý hàng đợi, mỗi phần tử khi thêm vào hàng đợi sẽ phải chờ các phần tử trước nó đã thêm thành công thì nó 
mới được thêm. 

1. Non-Blocking IO:
    +, None-blocking IO có nghĩa là xử lý đa luồng, thao tác A sẽ không bị phụ thuộc hoàn toàn vào thao tác B. Ví dụ trong hệ điều hành Window là xử 
lý đa luồng vì bạn có thể chạy nhiều chương trình cùng một lúc, thực hiện nhiều thao tác copy và xóa cho dù thao tác trước nó chưa hoàn thành.


V. Giải thích Code:
    IBUV là một thư viện hỗ trợ đa nền tảng với kĩ thuật đặc trưng Asynchronous I/O.
+, Đoạn 1:
        uv_tcp_t server;
        uv_tcp_init(loop, &server);
        uv_ip4_addr("0.0.0.0", 7000, &addr);
        uv_tcp_bind(&server, (const struct sockaddr*)&addr, 0);
        uv_listen((uv_stream_t*)&server, DEFAULT_BACKLOG, on_new_connection);
        uv_run(loop, UV_RUN_DEFAULT);

    1, uv_tcp_init(loop, &server) : Khởi tạo một vòng lặp loop vô vạn với biến socket là server.
    2, uv_ip4_addr("0.0.0.0", DEFAULT_PORT, &addr): Gán các thông tin như chấp nhận mọi IP của Server và Port = 7000 cho biến trung gian "addr"
    3. uv_tcp_bind(&server, (const struct sockaddr*)&addr, 0); : Gán thông tin cho server qua biến trung gian "addr"
    4. uv_listen((uv_stream_t*)&server, DEFAULT_BACKLOG, on_new_connection);
        --> server Lắng nghe client kết nối tới, khi có client kết nối tới, nó sẽ chạy vào hàm callback là on_new_connection.
    5. uv_run(loop, UV_RUN_DEFAULT); --> Bắt đầu chạy vòng loop với server vừa tạo bên trên, bắt đầu lắng nghe Event.

+, Đoạn 2:
    typedef struct {
    uv_tcp_t handle;
    http_parser parser;
    } client_t;

    void on_new_connection(uv_stream_t* server, int status) {
    if (status < 0) {
        fprintf(stderr, "New connection error %s\n", uv_strerror(status));
        exit(1);
    }
    client_t* client = (client_t*)malloc(sizeof(client_t));
    uv_tcp_init(server->loop, &client->handle);
    client->handle.data = client;                                                   //not understand

    if (uv_accept(server, (uv_stream_t*)&client->handle) == 0) {
        http_parser_init(&client->parser, HTTP_REQUEST);
        client->parser.data = client;                                               //not understand
        uv_read_start((uv_stream_t*)client, alloc_buffer, echo_read);
    } else {
        uv_close((uv_handle_t*)client, NULL);
    }
    printf("done on_new_connection\n");
    }

    1. Kiểm tra kết nối tới thành công không qua biến status.
    2. Khởi tạo biến client.
    3. Add biến Client vừa connect tới vào vòng loop của server vừa tạo ở Đoạn 1.
    4. Accept client kết nối tới, thực hiện việc read data qua hàm uv_read_start;
    5. Hàm uv_read_start() sẽ đọc dữ liệu từ một luồng tới, hàm callback "echo_read" sẽ được gọi khi không có data để đọc hoặc uv_read_stop được gọi.

+, Đoạn 3: 

    void echo_read(uv_stream_t* handle, ssize_t nread, const uv_buf_t* buf) {
        if (nread > 0) {
            printf("Data receive: ....\n%s\n",buf->base);
            uv_write_t* req = (uv_write_t*)malloc(sizeof(uv_write_t));
            uv_buf_t Mybuf = uv_buf_init(RESPONSE, sizeof(RESPONSE));
            client_t *client = (client_t *) handle->data;
            size_t parsed = http_parser_execute(&client->parser, &parser_settings, buf->base, nread);
            uv_write((uv_write_t*)req, (uv_stream_t*)client, &Mybuf, 1, echo_write);
            
        }
        if (nread < 0) {
            if (nread != UV_EOF)
                fprintf(stderr, "Read error %s\n", uv_err_name(nread));
            printf("Errir bread < 0\n");
            uv_close((uv_handle_t*)handle, NULL);
        }
        printf("Begin shutdown client\n");
        uv_shutdown_t *shutdown_req = (uv_shutdown_t *)malloc(sizeof(uv_shutdown_t));
        uv_shutdown(shutdown_req, handle, shutdown_cb);
        free(buf->base);
        }
    
    1. Đọc Data và in ra màn hình
    2. uv_write_t* req = (uv_write_t*)malloc(sizeof(uv_write_t)); --> Khởi tạo biến req, để truyền vào hàm uv_write();
    3. uv_buf_t Mybuf = uv_buf_init(RESPONSE, sizeof(RESPONSE)); --> Khởi tạo biến Mybuf có dữ liệu là chuỗi string RESPONSE
    4. size_t parsed = http_parser_execute(&client->parser, &parser_settings, buf->base, nread);    
    5. uv_write((uv_write_t*)req, (uv_stream_t*)client, &Mybuf, 1, echo_write);
        --> Gửi dữ liệu đi với hàm callback sau khi gửi xong là echo_write();
        và dữ liệu gửi đi là Mybuf;
    6. uv_shutdown() --> hàm này được gọi khi muốn kết thúc request clint gửi tới 
    7. uv_close() --> Request handle to be closed.