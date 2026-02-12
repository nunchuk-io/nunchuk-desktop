#!/usr/bin/env python3
"""
AppStrings.h String Adder Tool
A Tkinter GUI tool to add new strings to AppStrings.h file.
"""

import sys
import re
import os
import tkinter as tk
from tkinter import ttk, messagebox, scrolledtext


class AppStringsAdder:
    def __init__(self):
        self.app_strings_path = os.path.join(
            os.path.dirname(os.path.abspath(__file__)),
            "core/common/resources/AppStrings.h"
        )
        self.last_number = -1
        self.next_number = 0
        self.load_last_number()

    def load_last_number(self):
        """Load and find the last used STR_QML number from AppStrings.h"""
        try:
            if not os.path.exists(self.app_strings_path):
                return False, f"AppStrings.h not found at {self.app_strings_path}"

            with open(self.app_strings_path, 'r', encoding='utf-8') as f:
                content = f.read()

            # Find all STR_QML_XXXX patterns (only numeric ones)
            pattern = r'STR_QML_(\d+)'
            matches = re.findall(pattern, content)
            
            if matches:
                numbers = [int(m) for m in matches]
                self.last_number = max(numbers)
                self.next_number = self.last_number + 1
                return True, f"Found {len(set(numbers))} unique string IDs. Last: STR_QML_{self.last_number}, Next: STR_QML_{self.next_number}"
            else:
                self.last_number = -1
                self.next_number = 0
                return True, "No existing strings found. Next: STR_QML_0"
                
        except Exception as e:
            return False, f"Error loading file: {str(e)}"

    def escape_string(self, s):
        """Escape special characters for C++ string literal"""
        s = s.replace('\\', '\\\\')
        s = s.replace('"', '\\"')
        return s

    def generate_code(self, strings):
        """Generate DEFINE_STRING_PROPERTY lines for the given strings"""
        lines = []
        current_number = self.next_number
        
        for s in strings:
            s = s.strip()
            if s:
                escaped = self.escape_string(s)
                line = f'    DEFINE_STRING_PROPERTY(STR_QML_{current_number}, "{escaped}")'
                lines.append(line)
                current_number += 1
        
        return lines

    def add_strings(self, strings):
        """Add the strings to AppStrings.h file"""
        strings = [s.strip() for s in strings if s.strip()]
        
        if not strings:
            return False, "No strings to add."

        lines = self.generate_code(strings)
        new_code = '\n'.join(lines)

        try:
            with open(self.app_strings_path, 'r', encoding='utf-8') as f:
                content = f.read()

            # Match the public: section that comes after DEFINE_STRING_PROPERTY entries
            insert_pattern = r'(\npublic:\s*\n\s*static AppStrings)'
            match = re.search(insert_pattern, content)
            
            if match:
                insert_pos = match.start()
                new_content = content[:insert_pos] + '\n' + new_code + content[insert_pos:]
                
                with open(self.app_strings_path, 'w', encoding='utf-8') as f:
                    f.write(new_content)
                
                added_range = f"STR_QML_{self.next_number} to STR_QML_{self.next_number + len(lines) - 1}"
                self.load_last_number()
                return True, f"Successfully added {len(lines)} string(s)!\n{added_range}"
            else:
                return False, "Could not find insertion point in AppStrings.h"
                
        except Exception as e:
            return False, f"Failed to write: {str(e)}"


class AppStringsGUI:
    def __init__(self, root):
        self.root = root
        self.root.title("AppStrings.h String Adder")
        self.root.geometry("700x600")
        self.root.minsize(500, 400)
        
        self.adder = AppStringsAdder()
        self.setup_ui()
        self.refresh_status()

    def setup_ui(self):
        # Main frame with padding
        main_frame = ttk.Frame(self.root, padding="10")
        main_frame.pack(fill=tk.BOTH, expand=True)

        # Status frame
        status_frame = ttk.LabelFrame(main_frame, text="Status", padding="5")
        status_frame.pack(fill=tk.X, pady=(0, 10))

        self.status_label = ttk.Label(status_frame, text="Loading...", wraplength=650)
        self.status_label.pack(fill=tk.X)

        self.path_label = ttk.Label(status_frame, text=f"File: {self.adder.app_strings_path}", 
                                     wraplength=650, foreground="gray")
        self.path_label.pack(fill=tk.X, pady=(5, 0))

        # Input frame
        input_frame = ttk.LabelFrame(main_frame, text="Enter Strings (one per line)", padding="5")
        input_frame.pack(fill=tk.BOTH, expand=True, pady=(0, 10))

        self.input_text = scrolledtext.ScrolledText(input_frame, height=10, font=("Consolas", 10))
        self.input_text.pack(fill=tk.BOTH, expand=True)

        # Preview frame
        preview_frame = ttk.LabelFrame(main_frame, text="Preview", padding="5")
        preview_frame.pack(fill=tk.BOTH, expand=True, pady=(0, 10))

        self.preview_text = scrolledtext.ScrolledText(preview_frame, height=10, font=("Consolas", 10),
                                                       state=tk.DISABLED, background="#f5f5f5")
        self.preview_text.pack(fill=tk.BOTH, expand=True)

        # Buttons frame
        button_frame = ttk.Frame(main_frame)
        button_frame.pack(fill=tk.X)

        ttk.Button(button_frame, text="Refresh Status", command=self.refresh_status).pack(side=tk.LEFT, padx=(0, 5))
        ttk.Button(button_frame, text="Generate Preview", command=self.generate_preview).pack(side=tk.LEFT, padx=(0, 5))
        ttk.Button(button_frame, text="Clear", command=self.clear_all).pack(side=tk.LEFT, padx=(0, 5))
        
        self.add_button = ttk.Button(button_frame, text="Add to AppStrings.h", command=self.add_strings)
        self.add_button.pack(side=tk.RIGHT)

        # Bind input change to auto-preview
        self.input_text.bind('<KeyRelease>', lambda e: self.generate_preview())

    def refresh_status(self):
        success, message = self.adder.load_last_number()
        self.status_label.config(text=message, foreground="black" if success else "red")
        self.generate_preview()

    def get_input_strings(self):
        text = self.input_text.get("1.0", tk.END)
        return [line for line in text.split('\n') if line.strip()]

    def generate_preview(self):
        strings = self.get_input_strings()
        
        self.preview_text.config(state=tk.NORMAL)
        self.preview_text.delete("1.0", tk.END)
        
        if strings:
            lines = self.adder.generate_code(strings)
            preview = '\n'.join(lines)
            self.preview_text.insert("1.0", preview)
            self.preview_text.insert(tk.END, f"\n\n// Total: {len(lines)} string(s)")
        else:
            self.preview_text.insert("1.0", "// Enter strings above to see preview")
        
        self.preview_text.config(state=tk.DISABLED)

    def clear_all(self):
        self.input_text.delete("1.0", tk.END)
        self.generate_preview()

    def add_strings(self):
        strings = self.get_input_strings()
        
        if not strings:
            messagebox.showwarning("No Input", "Please enter at least one string.")
            return

        # Confirm
        lines = self.adder.generate_code(strings)
        confirm = messagebox.askyesno(
            "Confirm Add",
            f"Add {len(lines)} string(s) to AppStrings.h?\n\n"
            f"Range: STR_QML_{self.adder.next_number} to STR_QML_{self.adder.next_number + len(lines) - 1}"
        )
        
        if confirm:
            success, message = self.adder.add_strings(strings)
            if success:
                messagebox.showinfo("Success", message)
                self.clear_all()
                self.refresh_status()
            else:
                messagebox.showerror("Error", message)


def main():
    root = tk.Tk()
    
    # Set style
    style = ttk.Style()
    style.theme_use('clam')
    
    app = AppStringsGUI(root)
    root.mainloop()


if __name__ == '__main__':
    main()
