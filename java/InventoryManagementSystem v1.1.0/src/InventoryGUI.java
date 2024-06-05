//이 클래스가 메인 클래스 입니다. 
/*스마트 재고관리 프로그램 v1.0.0 
상점이나 회사에서 사용하는 재고 관리 시스템을 구현하는 프로그램입니다.
사용자는 상품을 추가하거나 사용자를 추가할 수 있으며, 재고를 변경하거나 파일로부터 상품을 불러오거나 파일로 상품을 저장할 수 있습니다.
[주요기능]
1. 재고 입출고 관리 
2. 재고 수준 모니터링
3. 데이터 동기화
4. GUI 콘솔창
5. 파일 입출력
6. 로그저장
[클래스 설계]
1. 상품 클래스 (Product)
속성: productID, name, price, quantity, category
메서드: 생성자, getter 및 setter
    -상품 ID
    -상품명
    - 가격
    - 수량
    - 카테고리
2. 사용자 클래스 (User)
속성: userID, name, role
메서드: 생성자, getter 및 setter
    - 사용자 ID
    - 사용자명
    - 역할 (관리자, 직원 등)
3. 재고 관리 클래스 (InventoryManager)
속성: 제품 목록 (Product 객체 리스트), 사용자 목록 (User 객체 리스트)
메서드: 재고 입출고 관리, 파일 입출력, 로그 기록
    - 상품 목록 (Product 객체 리스트)
    - 사용자 목록 (User 객체 리스트)
    - 재고 입출고 관리
    - 파일 입출력 기능
    - 로그 기록 기능 
4. GUI 콘솔 및 메인 클래스 (InventoryGUI)
속성: Scanner 객체
메서드: 메인 메뉴 표시, 사용자 입력 처리, 각 기능 호출

[프로그램 실행 흐름]
1. InventoryGUI 클래스의 displayMainMenu 메서드를 호출하여 메인 메뉴를 표시합니다.
2. 사용자가 원하는 작업을 선택하면 해당 기능을 수행하는 메서드를 호출합니다.
3. 각 메서드에서는 사용자로부터 필요한 정보를 입력받고, InventoryManager 클래스의 메서드를 호출하여 기능을 수행합니다.
4. InventoryManager 클래스의 메서드에서는 필요한 작업을 수행하고, 필요한 경우 로그를 남깁니다.
5. 사용자가 종료를 선택하면 프로그램을 종료합니다.

추가했으면 좋겠다고 추천받은 기능들
// 재고파일 상품 ID| 상품이름 | 수량 | 가격 | 카테고리 식으로 표현
// 사용자파일 사용자 ID| 사용자이름 | 역할 | 비밀번호 식으로 표현
// 
1. 사용자 입력에 대한 예외 처리
2. 파일 입출력 시 예외 처리
3. 로그 기록 기능 추가
4. 사용자 입력에 대한 유효성 검사
5. 상품 목록 및 사용자 목록 출력 기능 추가
6. 상품 목록 및 사용자 목록을 파일로 저장 및 불러오기 기능 추가
7. 상품 목록 및 사용자 목록을 데이터베이스에 저장 및 불러오기 기능 추가
8. 상품 검색 기능 추가
9. 사용자 검색 기능 추가
10. 상품 주문 및 판매 기능 추가
11. 사용자 권한 관리 기능 추가
12. 상품 카테고리 관리 기능 추가
13. 상품 재고 알림 기능 추가
14. 상품 재고 이력 조회 기능 추가
15. 상품 재고 이력 파일로 저장 및 불러오기 기능 추가
 */
import java.util.Scanner;
import java.io.IOException;

public class InventoryGUI {
    private InventoryManager manager;
    private Scanner scanner;
    private User loggedInUser;

    public InventoryGUI() {
        manager = new InventoryManager();
        scanner = new Scanner(System.in);
    }

    public void displayMainMenu() {
        while (true) {
            System.out.println("┌────────────────────────────┐");
            System.out.println("│      재고 관리 시스템      │");
            System.out.println("├────────────────────────────┤");
            System.out.println("│  1. 상품 추가              │");
            System.out.println("│  2. 사용자 추가            │");
            System.out.println("│  3. 재고 변경              │");
            System.out.println("│  4. 현재 모든 상품 보기    │");
            System.out.println("│  5. 로그아웃               │");
            System.out.println("│  6. 종료                  │");
            System.out.println("└────────────────────────────┘");
            System.out.print("원하는 작업을 선택하세요: ");

            int choice = scanner.nextInt();
            scanner.nextLine();  // 개행 문자 처리

            switch (choice) {
                case 1:
                    if (loggedInUser != null && loggedInUser.getRole().equals("관리자")) {
                        addProduct();
                    } else {
                        System.out.println("권한이 없습니다.");
                    }
                    break;
                case 2:
                    if (loggedInUser != null && loggedInUser.getRole().equals("관리자")) {
                        addUser();
                    } else {
                        System.out.println("권한이 없습니다.");
                    }
                    break;
                case 3:
                    if (loggedInUser != null && (loggedInUser.getRole().equals("관리자") || loggedInUser.getRole().equals("직원"))) {
                        updateProductQuantity();
                    } else {
                        System.out.println("권한이 없습니다.");
                    }
                    break;
                case 4:
                    manager.displayAllProducts();
                    break;
                case 5:
                    logout();
                    return;
                case 6:
                    System.out.println("\n프로그램을 종료합니다. 감사합니다!");
                    return;
                default:
                    System.out.println("\n잘못된 선택입니다. 다시 시도해주세요.");
            }
        }
    }

    public void login() {
        while (true) {
            System.out.println(" _    _  _   __ _   _     ___                                              _              _   ");
            System.out.println("| |  | || | / /| | | |   |_  |                                            (_)            | |  ");
            System.out.println("| |  | || |/ / | | | |     | |  __ _ __   __  __ _   _ __   _ __   ___     _   ___   ___ | |_ ");
            System.out.println("| |/\\| ||    \\ | | | |     | | / _` |\\ \\ / / / _` | | '_ \\ | '__| / _ \\   | | / _ \\ / __|| __|");
            System.out.println("\\  /\\  /| |\\  \\| |_| | /\\__/ /| (_| | \\ V / | (_| | | |_) || |   | (_) |  | ||  __/| (__ | |_ ");
            System.out.println(" \\/  \\/ \\_| \\_/ \\___/  \\____/  \\__,_|  \\_/   \\__,_| | .__/ |_|    \\___/   | | \\___| \\___| \\__|");
            System.out.println("                                                    | |                  _/ |                 ");
            System.out.println("                                                    |_|                 |__/                  ");
            System.out.println("┌────────────────────────────┐");
            System.out.println("│        사용자 로그인       │");
            System.out.println("├────────────────────────────┤");
            System.out.print("│ 사용자 ID: ");
            int userID = scanner.nextInt();
            scanner.nextLine();  // 개행 문자 처리

            System.out.print("│ 비밀번호: ");
            String password = scanner.nextLine();

            loggedInUser = manager.authenticateUser(userID, password);
            if (loggedInUser != null) {
                System.out.println("└────────────────────────────┘");
                System.out.println("로그인 성공! 환영합니다, " + loggedInUser.getName() + "님.");
                break;
            } else {
                System.out.println("└────────────────────────────┘");
                System.out.println("로그인 실패. 다시 시도하세요.");
            }
        }
    }

    private void logout() {
        loggedInUser = null;
        System.out.println("로그아웃 되었습니다.");
        login();
        displayMainMenu();
    }

    private void addProduct() {
        System.out.println("┌────────────────────────────┐");
        System.out.println("│        상품 추가           │");
        System.out.println("├────────────────────────────┤");
        System.out.print("│ 상품 ID: ");
        int productID = scanner.nextInt();
        scanner.nextLine();  // 개행 문자 처리

        System.out.print("│ 상품명: ");
        String name = scanner.nextLine();

        System.out.print("│ 가격: ");
        double price = scanner.nextDouble();
        scanner.nextLine();  // 개행 문자 처리

        System.out.print("│ 수량: ");
        int quantity = scanner.nextInt();
        scanner.nextLine();  // 개행 문자 처리

        System.out.print("│ 카테고리: ");
        String category = scanner.nextLine();

        Product product = new Product(productID, name, price, quantity, category);
        manager.addProduct(product);
        System.out.println("└────────────────────────────┘");
        System.out.println("상품이 추가되었습니다.");
    }

    private void addUser() {
        System.out.println("┌────────────────────────────┐");
        System.out.println("│        사용자 추가         │");
        System.out.println("├────────────────────────────┤");
        System.out.print("│ 사용자 ID: ");
        int userID = scanner.nextInt();
        scanner.nextLine();  // 개행 문자 처리

        System.out.print("│ 사용자명: ");
        String name = scanner.nextLine();

        System.out.print("│ 역할: ");
        String role = scanner.nextLine();

        System.out.print("│ 비밀번호: ");
        String password = scanner.nextLine();

        User user = new User(userID, name, role, password);
        manager.addUser(user);
        System.out.println("└────────────────────────────┘");
        System.out.println("사용자가 추가되었습니다.");
    }

    private void updateProductQuantity() {
        System.out.println("┌────────────────────────────┐");
        System.out.println("│        재고 변경           │");
        System.out.println("├────────────────────────────┤");
        System.out.print("│ 상품 ID: ");
        int productID = scanner.nextInt();
        scanner.nextLine();  // 개행 문자 처리

        System.out.print("│ 변경 유형 (추가/제거): ");
        String changeType = scanner.nextLine();

        System.out.print("│ 변경 수량: ");
        int quantityChange = scanner.nextInt();
        scanner.nextLine();  // 개행 문자 처리

        boolean isAdding = changeType.equals("추가");
        manager.updateProductQuantity(productID, quantityChange, isAdding);
        System.out.println("└────────────────────────────┘");
        System.out.println("재고가 변경되었습니다.");
    }

    public static void main(String[] args) {
        InventoryGUI gui = new InventoryGUI();
        gui.login();
        gui.displayMainMenu();
    }
}
