using System.Text.RegularExpressions;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Media;

public class CreateJxxClassDialog : Window
{
    private ComboBox typeBox;
    private TextBox nameBox;
    private TextBox namespaceBox;
    private TextBlock validationText;
    private Button createButton;

    public string ClassName => nameBox.Text;
    public string NamespaceValue => namespaceBox.Text;
    public bool IsInterface => typeBox.SelectedIndex == 1;

    public CreateJxxClassDialog(string defaultNamespace)
    {
        Title = "Create JXX Class";
        Width = 400;
        Height = 250;

        var panel = new StackPanel { Margin = new Thickness(10) };

        panel.Children.Add(new TextBlock { Text = "Type:" });
        typeBox = new ComboBox { ItemsSource = new[] { "Class", "Interface" }, SelectedIndex = 0 };
        typeBox.SelectionChanged += (_, __) => Validate();
        panel.Children.Add(typeBox);

        panel.Children.Add(new TextBlock { Text = "Name:" });
        nameBox = new TextBox();
        nameBox.TextChanged += (_, __) => Validate();
        panel.Children.Add(nameBox);

        panel.Children.Add(new TextBlock { Text = "Namespace:" });
        namespaceBox = new TextBox { Text = defaultNamespace };
        namespaceBox.TextChanged += (_, __) => Validate();
        panel.Children.Add(namespaceBox);

        validationText = new TextBlock { Foreground = Brushes.Red };
        panel.Children.Add(validationText);

        createButton = new Button { Content = "Create", IsEnabled = false };
        createButton.Click += (_, __) => { DialogResult = true; Close(); };

        panel.Children.Add(createButton);

        Content = panel;
    }

    private void Validate()
    {
        string name = nameBox.Text;
        bool isInterface = typeBox.SelectedIndex == 1;

        if (string.IsNullOrWhiteSpace(name))
        {
            SetError("Name required");
            return;
        }

        if (isInterface)
        {
            if (!Regex.IsMatch(name, @"^I[A-Z][A-Za-z0-9]*$"))
            {
                SetError("Interface must start with I");
                return;
            }
        }
        else
        {
            if (!Regex.IsMatch(name, @"^[A-Z][A-Za-z0-9]*$"))
            {
                SetError("Class must be PascalCase");
                return;
            }
        }

        validationText.Text = "";
        createButton.IsEnabled = true;
    }

    private void SetError(string msg)
    {
        validationText.Text = msg;
        createButton.IsEnabled = false;
    }
}