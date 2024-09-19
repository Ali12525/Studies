using System;
using System.Collections.Generic;
using System.Diagnostics.Metrics;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Magazine.Core.Models
{
    internal class Product
    {
        private int id;
        private string definition;
        private string name;
        private int price;

        public int Id
        {
            get { return id; }
            set { id = value; }
        }

        public string Definition
        {
            get { return definition; }
            set { definition = value; }
        }

        public string Name
        {
            get { return name; }
            set { name = value; }
        }

        public int Price
        {
            get { return price; }
            set { price = value; }
        }
    }
}
