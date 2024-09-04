import java.util.ArrayList;
import java.util.Scanner;

// 고객 정보 클래스
class Customer {
    private String name;
    private String email;

    public Customer(String name, String email) {
        this.name = name;
        this.email = email;
    }

    public String getName() {
        return name;
    }

    public String getEmail() {
        return email;
    }
}

// 상품 클래스
class Product {
    private String isbn;
    private String name;
    private double price;
    private String author;
    private String description;
    private String category;
    private String publicationDate;

    public Product(String isbn, String name, double price, String author, String description, String category, String publicationDate) {
        this.isbn = isbn;
        this.name = name;
        this.price = price;
        this.author = author;
        this.description = description;
        this.category = category;
        this.publicationDate = publicationDate;
    }

    public String getIsbn() {
        return isbn;
    }

    public String getName() {
        return name;
    }

    public double getPrice() {
        return price;
    }

    public String getAuthor() {
        return author;
    }

    public String getDescription() {
        return description;
    }

    public String getCategory() {
        return category;
    }

    public String getPublicationDate() {
        return publicationDate;
    }

    @Override
    public String toString() {
        return isbn + " | " + name + " | " + price + " | " + author + " | " + description + " | " + category + " | " + publicationDate;
    }
}

// 장바구니 항목 클래스
class CartItem {
    private Product product;
    private int quantity;

    public CartItem(Product product, int quantity) {
        this.product = product;
        this.quantity = quantity;
    }

    public Product getProduct() {
        return product;
    }

    public int getQuantity() {
        return quantity;
    }

    public void setQuantity(int quantity) {
        this.quantity = quantity;
    }

    public double getTotalPrice() {
        return product.getPrice() * quantity;
    }
}

// 장바구니 클래스
class ShoppingCart {
    private ArrayList<CartItem> items;

    public ShoppingCart() {
        this.items = new ArrayList<>();
    }

    public void addItem(Product product, int quantity) {
        for (CartItem item : items) {
            if (item.getProduct().getIsbn().equals(product.getIsbn())) {
                item.setQuantity(item.getQuantity() + quantity);
                return;
            }
        }
        items.add(new CartItem(product, quantity));
    }

    public void removeItem(String isbn) {
        items.removeIf(item -> item.getProduct().getIsbn().equals(isbn));
    }

    public void reduceItemQuantity(String isbn, int quantity) {
        for (CartItem item : items) {
            if (item.getProduct().getIsbn().equals(isbn)) {
                int newQuantity = item.getQuantity() - quantity;
                if (newQuantity > 0) {
                    item.setQuantity(newQuantity);
                } else {
                    removeItem(isbn);
                }
                return;
            }
        }
    }

    public void clearCart() {
        items.clear();
    }

    public void showCart() {
        if (items.isEmpty()) {
            System.out.println("장바구니가 비어 있습니다.");
        } else {
            for (CartItem item : items) {
                System.out.println(item.getProduct().getName() + " - 수량: " + item.getQuantity() + " - 총 가격: " + item.getTotalPrice());
            }
        }
    }

    public double getTotalPrice() {
        double total = 0;
        for (CartItem item : items) {
            total += item.getTotalPrice();
        }
        return total;
    }
}

// 영수증 클래스
class Receipt {
    public static void printReceipt(ShoppingCart cart) {
        System.out.println("===== 영수증 =====");
        cart.showCart();
        System.out.println("총 가격: " + cart.getTotalPrice());
        System.out.println("=================");
    }
}

// 관리자 클래스
class Admin {
    private String username;
    private String password;

    public Admin(String username, String password) {
        this.username = username;
        this.password = password;
    }

    public boolean login(String username, String password) {
        return this.username.equals(username) && this.password.equals(password);
    }
}

// 메인 클래스
public class OnlineBookstore {
    public static void main(String[] args) {
        Scanner scanner = new Scanner(System.in);
        ShoppingCart cart = new ShoppingCart();
        Admin admin = new Admin("admin", "password");

        while (true) {
            System.out.println("**********************************************");
            System.out.println("Welcome to Shopping Mall");
            System.out.println("Welcome to Book Market!");
            System.out.println("**********************************************");
            System.out.println("1. 고객 정보 확인하기        4. 바구니에 항목 추가하기");
            System.out.println("2. 장바구니 상품 목록 보기     5. 장바구니에 항목수량 줄이기");
            System.out.println("3. 장바구니 비우기           6. 장바구니의 항목 삭제하기");
            System.out.println("7. 영수증 표시하기          8. 종료");
            System.out.println("********************************************************");
            System.out.print("메뉴 번호를 선택하세요: ");
            int choice = scanner.nextInt();
            scanner.nextLine();  // 개행 문자 제거

            switch (choice) {
                case 1:
                    System.out.print("이름: ");
                    String name = scanner.nextLine();
                    System.out.print("이메일: ");
                    String email = scanner.nextLine();
                    Customer customer = new Customer(name, email);
                    System.out.println("고객 이름: " + customer.getName());
                    System.out.println("고객 이메일: " + customer.getEmail());
                    break;
                case 2:
                    cart.showCart();
                    break;
                case 3:
                    cart.clearCart();
                    System.out.println("장바구니를 비웠습니다.");
                    break;
                case 4:
                    System.out.println("장바구니 항목 추가");
                    System.out.print("ISBN: ");
                    String isbn = scanner.nextLine();
                    System.out.print("상품 이름: ");
                    String productName = scanner.nextLine();
                    System.out.print("상품 가격: ");
                    double productPrice = scanner.nextDouble();
                    scanner.nextLine(); // 개행 문자 제거
                    System.out.print("저자: ");
                    String author = scanner.nextLine();
                    System.out.print("설명: ");
                    String description = scanner.nextLine();
                    System.out.print("카테고리: ");
                    String category = scanner.nextLine();
                    System.out.print("출판일: ");
                    String publicationDate = scanner.nextLine();
                    System.out.print("수량: ");
                    int quantity = scanner.nextInt();
                    scanner.nextLine();  // 개행 문자 제거

                    cart.addItem(new Product(isbn, productName, productPrice, author, description, category, publicationDate), quantity);
                    System.out.println("상품을 추가했습니다.");
                    System.out.println(isbn + " | " + productName + " | " + productPrice + " | " + quantity + " | " + author + " | " + description + " | " + category + " | " + publicationDate);
                    break;
                case 5:
                    System.out.print("ISBN: ");
                    String reduceIsbn = scanner.nextLine();
                    System.out.print("수량 줄이기: ");
                    int reduceQuantity = scanner.nextInt();
                    scanner.nextLine();  // 개행 문자 제거
                    cart.reduceItemQuantity(reduceIsbn, reduceQuantity);
                    System.out.println("상품 수량을 줄였습니다.");
                    break;
                case 6:
                    System.out.print("ISBN: ");
                    String removeIsbn = scanner.nextLine();
                    cart.removeItem(removeIsbn);
                    System.out.println("상품을 삭제했습니다.");
                    break;
                case 7:
                    Receipt.printReceipt(cart);
                    break;
                case 8:
                    System.out.println("프로그램을 종료합니다.");
                    scanner.close();
                    return;
                default:
                    System.out.println("잘못된 선택입니다.");
                    break;
            }
        }
    }
}
