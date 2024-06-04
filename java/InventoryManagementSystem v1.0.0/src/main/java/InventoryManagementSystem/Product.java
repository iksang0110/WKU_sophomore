package WKU_sophomore.java.InventoryManagementSystem.src.main.java.InventoryManagementSystem;

import java.io.Serializable;

public class Product implements Serializable {
    private static final long serialVersionUID = 1L;
    private String productId;
    private String name;
    private double price;
    private int quantity;
    private String category;

    public Product(String productId, String name, double price, int quantity, String category) {
        this.productId = productId;
        this.name = name;
        this.price = price;
        this.quantity = quantity;
        this.category = category;
    }

    public String getProductId() { return productId; }
    public void setProductId(String productId) { this.productId = productId; }
    public String getName() { return name; }
    public void setName(String name) { this.name = name; }
    public double getPrice() { return price; }
    public void setPrice(double price) { this.price = price; }
    public int getQuantity() { return quantity; }
    public void setQuantity(int quantity) { this.quantity = quantity; }
    public String getCategory() { return category; }
    public void setCategory(String category) { this.category = category; }

    @Override
    public String toString() {
        return String.format("ID: %s, 이름: %s, 가격: %.2f, 수량: %d, 카테고리: %s",
                productId, name, price, quantity, category);
    }
}
