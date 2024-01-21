import csv
# Card Number, Title, Side, Operation Value, Categroy
with open('FTP.csv', newline='') as csv_file:
    reader = csv.reader(csv_file, delimiter=' ')
    titles={}
    sides={}
    operation_values={}
    categories= {}
    for row in reader:
        card_number, title, side, operation_value, category = row[0].split(',')
