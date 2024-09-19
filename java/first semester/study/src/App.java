public class App {
    public static void main(String[] args) throws Exception {
        PlusMinus ob1 = new PlusMinus();
        String ssum, sminus;
        ssum = ob1.plus(50, 30);
        sminus = ob1.minus(50, 30);
        System.out.println(ssum);
        System.out.println(sminus);
    }
}
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
class PlusMinus {

    int add, sub;
    public String plus(int x, int y){
        add = x + y;
        return "두 수의 합 : " + add;
    }
    public String minus(int x, int y){
        sub = x - y;
        return "두 수의 차 : " + sub; 
    }

    Expense 클래스
    Expense 클래스는 지출 내역을 나타내는 데이터를 캡슐화합니다. 각 지출 내역은 사용목적, 사용금액, 사용일 세 가지 주요 정보로 구성됩니다.
    
    필드
    
    private String purpose: 지출의 목적을 저장하는 문자열입니다. 예를 들어, "식사", "교통비" 등이 있을 수 있습니다.
    private int amount: 지출된 금액을 저장하는 정수입니다. 이 필드는 금액의 정확도를 유지하기 위해 정수형으로 선언됩니다.
    private String date: 지출이 이루어진 날짜를 저장하는 문자열입니다. 일반적으로 "YYYY-MM-DD" 형식으로 저장됩니다.
    메소드
    
    public Expense(String purpose, int amount, String date): 생성자 메소드로, 객체를 생성할 때 지출 목적, 금액, 날짜를 초기화합니다.
    Getter 및 Setter: 각 필드의 값을 검색하거나 설정할 수 있는 메소드를 제공합니다. 이는 클래스의 캡슐화를 유지하며 외부에서 직접 필드에 접근하는 것을 방지합니다.
    public String getPurpose(), public void setPurpose(String purpose)
    public int getAmount(), public void setAmount(int amount)
    public String getDate(), public void setDate(String date)
    public String displayExpense(): 지출 내역의 상세 정보를 포맷된 문자열로 반환합니다. 이 메소드는 지출 내역을 사용자에게 보여줄 때 사용됩니다.
    메인 애플리케이션 (ExpenseTracker 클래스 내 메인 메소드)
    이 부분은 사용자 인터페이스와 프로그램의 흐름을 제어합니다. 사용자로부터 입력을 받아 Expense 객체를 생성하고 관리합니다.
    
    주요 기능
    
    addExpense(): 사용자로부터 지출 목적, 금액, 날짜를 입력받아 새로운 Expense 객체를 생성하고 이를 리스트에 추가합니다.
    viewExpenses(): 저장된 모든 지출 내역을 조회하여 보여줍니다. 지출 내역이 없는 경우, 적절한 메시지를 출력합니다.
    메인 루프: 사용자로부터의 입력을 받아 지출 내역 추가, 조회, 또는 프로그램 종료 등의 작업을 수행합니다. 이 루프는 사용자가 종료를 선택할 때까지 계속됩니다.
    }