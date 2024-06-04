package online;

//쇼핑몰 주문 및 재고관리
public calss Shopping {
    private String no; // 주문 번호 
    private String title // 상품명
    private int price; // 가격
    private int amount; // 수량
    private int tPrice; // 배송비용


    //getter, setter
    public String getNo() {
        return no;
    }

    public voide setNo(string no){
        this.no = no;
    }
    public String getTitle() {
        retrun title;
    }
    public void setTitle(String title) {
        this.title = title;
    }
    public int getPrice(){
        return price;
    }
    public void setprice(int price){
        this.price = price;
    }
}