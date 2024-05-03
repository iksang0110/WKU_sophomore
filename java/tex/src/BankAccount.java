//문제 2번. 
//계좌 정보를 저장할 수 있는 클래스를 생성
//해당 클래스의 필드는 private로 선언하고, owner(String)와 balance(long)이름으로 지정.
//위 모든 필드에 대한 getter와 setter 구현
//위 모든 필드를 사용 가능한 모든 생성자(생성메서드)구현
//금액을 저축하는 deposit()메소드 기능을 작성, 또한 금액을 인출하는 Withdraw()메소드 작성

public class BankAccount {
    // 계좌의 소유자와 잔액을 저장하는 private 필드
    private String owner;
    private long balance;

    // 모든 필드를 사용하는 생성자
    public BankAccount(String owner, long balance) {
        this.owner = owner;
        this.balance = balance;
    }

    // owner 필드에 대한 getter와 setter
    public String getOwner() {
        return owner;
    }

    public void setOwner(String owner) {
        this.owner = owner;
    }

    // balance 필드에 대한 getter와 setter
    public long getBalance() {
        return balance;
    }

    public void setBalance(long balance) {
        this.balance = balance;
    }

    // 금액을 저축하는 메소드
    public void deposit(long amount) {
        if (amount > 0) {
            balance += amount;
            System.out.println(amount + "원이 입금되었습니다. 현재 잔액: " + balance + "원");
        } else {
            System.out.println("입금 금액은 0보다 커야 합니다.");
        }
    }

    // 금액을 인출하는 메소드
    public void withdraw(long amount) {
        if (amount > 0 && amount <= balance) {
            balance -= amount;
            System.out.println(amount + "원이 출금되었습니다. 현재 잔액: " + balance + "원");
        } else if (amount > balance) {
            System.out.println("잔액이 부족합니다. 출금을 시도한 금액: " + amount + "원, 현재 잔액: " + balance + "원");
        } else {
            System.out.println("출금 금액은 0보다 커야 합니다.");
        }
    }
}
