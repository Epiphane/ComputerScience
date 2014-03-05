import javax.swing.*;
import javax.swing.border.EtchedBorder;
import java.awt.event.*;
import java.awt.*;
import java.text.NumberFormat;

public class SodaMachineFrame extends JFrame {
   private JTextField amountDeposited, changeReturned, totalSales;
   private JPanel sodaPanel;
   private SodaMachine sodaMachine;
   private JButton coinReturnButton;
   
   public SodaMachineFrame(SodaMachine sodaMachine) {
      this.sodaMachine = sodaMachine;
      this.setResizable(false);
	      
      // Listener
      class CoinReturnListener implements ActionListener {
         public void actionPerformed(ActionEvent event) {
            returnDeposits();
         }
      }
      
      getContentPane().setPreferredSize(new Dimension(450,300));
      
      // Center panel
      JPanel centerPanel = new JPanel();
      centerPanel.setPreferredSize(new Dimension(150, 300));
      centerPanel.setLayout(new GridLayout(0,1));
      
      JPanel depositPanel = new JPanel();
      amountDeposited = new JTextField(6);
      amountDeposited.setHorizontalAlignment(JTextField.RIGHT);
      amountDeposited.setEditable(false);
      amountDeposited.setText(NumberFormat.getCurrencyInstance().format(0));
      coinReturnButton = new JButton("Coin return");
      coinReturnButton.setEnabled(false);
      coinReturnButton.addActionListener(new CoinReturnListener());
      depositPanel.add(new JLabel("Amount Deposited"));
      depositPanel.add(amountDeposited);
      depositPanel.add(coinReturnButton);
      depositPanel.setBorder(new EtchedBorder(EtchedBorder.LOWERED));
      centerPanel.add(depositPanel);
      
      JPanel changePanel = new JPanel();
      changeReturned = new JTextField(6);
      changeReturned.setHorizontalAlignment(JTextField.RIGHT);
      changeReturned.setEditable(false);
      changeReturned.setText(NumberFormat.getCurrencyInstance().format(0));
      changePanel.add(new JLabel("Change Returned"));
      changePanel.add(changeReturned);
      changePanel.setBorder(new EtchedBorder(EtchedBorder.LOWERED));
      centerPanel.add(changePanel);
      
      JPanel salesPanel = new JPanel();
      totalSales = new JTextField(6);
      totalSales.setHorizontalAlignment(JTextField.RIGHT);
      totalSales.setEditable(false);
      totalSales.setText(NumberFormat.getCurrencyInstance().format(0));
      salesPanel.add(new JLabel("Total Sales"));
      salesPanel.add(totalSales);
      salesPanel.setBorder(new EtchedBorder(EtchedBorder.LOWERED));
      centerPanel.add(salesPanel);
      
      getContentPane().add(centerPanel, BorderLayout.CENTER);
      
      // East panel with coins
      JPanel eastPanel = new JPanel();
      eastPanel.setPreferredSize(new Dimension(150, 300));
      eastPanel.setLayout(new GridLayout(0,1));
      createMoneyButtons(eastPanel);
      getContentPane().add(eastPanel, BorderLayout.EAST);
      
      // West panel with sodas
      sodaPanel = new JPanel();
      sodaPanel.setPreferredSize(new Dimension(150, 300));
      sodaPanel.setLayout(new GridLayout(0,1));
      createSodaButtons(sodaPanel, sodaMachine);
      
      getContentPane().add(sodaPanel, BorderLayout.WEST);

      pack();
      setVisible(true);
   }
   
   private void createMoneyButtons(JPanel panel) {
      JButton nickel = new JButton("Nickel");
      JButton dime = new JButton("Dime");
      JButton quarter = new JButton("Quarter");
      JButton halfDollar = new JButton("Half Dollar");
      JButton dollar = new JButton("Dollar");
      
      // Listener
      class CoinListener implements ActionListener {
         private int value;
         public CoinListener(int value) {
            super();
            this.value = value;
         }
         
         public void actionPerformed(ActionEvent event) {
            deposit(value);
         }
      }
      
      nickel.addActionListener(new CoinListener(5));
      dime.addActionListener(new CoinListener(10));
      quarter.addActionListener(new CoinListener(25));
      halfDollar.addActionListener(new CoinListener(50));
      dollar.addActionListener(new CoinListener(100));
      
      panel.add(nickel);
      panel.add(dime);
      panel.add(quarter);
      panel.add(halfDollar);
      panel.add(dollar);
   }
   
   private void createSodaButtons(JPanel panel, SodaMachine sodaMachine) {
	  // Listener
	  class SodaListener implements ActionListener {
	     private int sodaIndex;
	     public SodaListener(int sodaIndex) {
	        super();
	        this.sodaIndex = sodaIndex;
	     }
	     
	     public void actionPerformed(ActionEvent event) {
	        dispense(sodaIndex);
	     }
	  }
	  
	  
      for(int i = 0; i < sodaMachine.getCount(); i++) {
         JButton button = new JButton(sodaMachine.getSodaName(i));
         button.setEnabled(false);
         button.addActionListener(new SodaListener(i));
         panel.add(button);
      }
   }
   
   private void updateSodaButtons(boolean active) {
      for(int i = 0; i < sodaPanel.getComponentCount(); i++) {
         sodaPanel.getComponent(i).setEnabled(active);
	     if(active && sodaMachine.getSodaCount(i) <= 0) {
	        sodaPanel.getComponent(i).setEnabled(false);
	     }
      }
   }
   
   private void deposit(int value) {
      sodaMachine.deposit(value);
      amountDeposited.setText(NumberFormat.getCurrencyInstance().format(((double)sodaMachine.getDeposits())/100));
      updateSodaButtons(sodaMachine.sufficientFunds());
      coinReturnButton.setEnabled(true);
   }
   
   private void dispense(int sodaIndex) {
      changeReturned.setText(NumberFormat.getCurrencyInstance().format(((double)sodaMachine.dispenseSoda(sodaIndex))/100));
      updateSodaButtons(sodaMachine.sufficientFunds());
      totalSales.setText(NumberFormat.getCurrencyInstance().format(((double)sodaMachine.getSales())/100));
      amountDeposited.setText(NumberFormat.getCurrencyInstance().format(((double)sodaMachine.getDeposits())/100));
      coinReturnButton.setEnabled(false);
   }
   
   private void returnDeposits() {
      amountDeposited.setText("$0.00");
      changeReturned.setText(NumberFormat.getCurrencyInstance().format(((double)sodaMachine.returnDeposits())/100));
      coinReturnButton.setEnabled(false);
   }
}