using Microsoft.EntityFrameworkCore;
using ShopApp.Models;
using ShopApp.Services;
using ShopApp.WebApi.Infrastructure;

namespace ShopApp.WebApi.Services
{
    public class DataBaseProductService : IProductService
    {
        private readonly ApplicationContext _context;

        public DataBaseProductService(ApplicationContext context)
        {
            _context = context;
        }

        public Product? Add(Product product)
        {
            try
            {
                _context.Products.Add(product);
                _context.SaveChanges();
                return product;
            }
            catch
            {
                return null;
            }
        }

        public Product? Remove(Guid id)
        {
            var product = _context.Products.Find(id);
            if (product == null) return null;

            _context.Products.Remove(product);
            _context.SaveChanges();
            return product;
        }

        public Product? Edit(Product product)
        {
            try
            {
                _context.Entry(product).State = EntityState.Modified;
                _context.SaveChanges();
                return product;
            }
            catch
            {
                return null;
            }
        }

        public Product? Search(Guid id)
        {
            return _context.Products.Find(id);
        }
    }
}
