using System;
using System.Linq;
using ShopApp.Models;
using ShopApp.WebApi.DB;
using ShopApp.Core.DB;

namespace ShopApp.Services
{
    /// <summary>
    /// Provides services for managing products using SQLite database.
    /// </summary>
    public class ProductService : IProductService
    {
        private readonly IDataBase _database;

        /// <summary>
        /// Initializes a new instance of the <see cref="ProductService"/> class.
        /// </summary>
        public ProductService(IDataBase database)
        {
            _database = database;
        }

        /// <summary>
        /// Adds a new product to the database.
        /// </summary>
        /// <param name="product">The product to add.</param>
        /// <returns>The added product if successful; otherwise, null.</returns>
        public Product? Add(Product product)
        {
            try
            {
                _database.InsertProduct(product);
                return product;
            }
            catch (Exception ex)
            {
                Console.WriteLine($"Error adding product: {ex.Message}");
                return null;
            }
        }

        /// <summary>
        /// Removes a product from the database by its unique identifier.
        /// </summary>
        /// <param name="id">The unique identifier of the product to remove.</param>
        /// <returns>The removed product if found; otherwise, null.</returns>
        public Product? Remove(Guid id)
        {
            var product = Search(id);
            if (product != null)
            {
                try
                {
                    _database.DeleteProduct(id);
                    return product;
                }
                catch (Exception ex)
                {
                    Console.WriteLine($"Error removing product: {ex.Message}");
                }
            }
            return null;
        }

        /// <summary>
        /// Edits an existing product in the database.
        /// </summary>
        /// <param name="product">The product with updated information.</param>
        /// <returns>The updated product if successful; otherwise, null.</returns>
        public Product? Edit(Product product)
        {
            // Проверяем, существует ли продукт в базе данных
            if (Search(product.Id) == null)
            {
                return null;
            }

            try
            {
                _database.UpdateProduct(product);
                return product;
            }
            catch (Exception ex)
            {
                Console.WriteLine($"Error editing product: {ex.Message}");
                return null;
            }
        }

        /// <summary>
        /// Searches for a product in the database by its unique identifier.
        /// </summary>
        /// <param name="id">The unique identifier of the product to search for.</param>
        /// <returns>The found product if it exists; otherwise, null.</returns>
        public Product? Search(Guid id)
        {
            try
            {
                var products = _database.GetProducts();
                return products.FirstOrDefault(p => p.Id == id);
            }
            catch (Exception ex)
            {
                Console.WriteLine($"Error searching product: {ex.Message}");
                return null;
            }
        }
    }
}