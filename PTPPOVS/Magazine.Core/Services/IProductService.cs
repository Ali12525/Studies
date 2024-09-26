using Magazine.Core.Models;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Magazine.Core.Services
{
    public interface IProductService
    {
        void Add(Product product);
        void Remove(Product product);
        void Edit(Product product);
        void Search(Product product);
    }
}
