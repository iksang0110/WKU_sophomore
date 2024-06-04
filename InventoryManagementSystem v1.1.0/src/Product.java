public class Product {
    private int productID;
    private String name;
    private double price;
    private int quantity;
    private String category;

    // 필요한 생성자 추가
    public Product(int productID, String name, double price, int quantity, String category) {
        this.productID = productID;
        this.name = name;
        this.price = price;
        this.quantity = quantity;
        this.category = category;
    }

    // 기본 생성자
    public Product() {}

    // Getter 및 Setter 메서드
    public int getProductID() {
        return productID;
    }

    public void setProductID(int productID) {
        this.productID = productID;
    }

    public String getName() {
        return name;
    }

    public void setName(String name) {
        this.name = name;
    }

    public double getPrice() {
        return price;
    }

    public void setPrice(double price) {
        this.price = price;
    }

    public int getQuantity() {
        return quantity;
    }

    public void setQuantity(int quantity) {
        this.quantity = quantity;
    }

    public String getCategory() {
        return category;
    }

    public void setCategory(String category) {
        this.category = category;
    }
}
